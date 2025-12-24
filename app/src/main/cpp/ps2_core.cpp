// ps2_core.cpp
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring> // for std::memcpy
#include "ps2_core.h"

// -----------------------------
// Configuration and state
// -----------------------------
static const uint32_t BIOS_BASE = 0xBFC00000;

static uint32_t PC = BIOS_BASE;        // Program Counter starts at BIOS entry
static uint64_t cycles = 0;            // Cycle counter
static bool biosLoaded = false;        // True when main ROM is present
static bool halted = false;            // Halt on errors
static std::string lastError;          // Last error message (for debug)

static uint32_t regs[32] = {0};        // EE GPRs (simplified for v0.2)

// BIOS parts (we primarily execute from ROM for now)
static std::vector<uint8_t> biosROM;   // Main BIOS image
static std::vector<uint8_t> biosROM1;  // Auxiliary BIOS 1
static std::vector<uint8_t> biosROM2;  // Auxiliary BIOS 2
static std::vector<uint8_t> biosEROM;  // Extended ROM
static std::vector<uint8_t> biosNVM;   // NVM (not executable)
static std::vector<uint8_t> biosMEC;   // MEC (not executable)

// -----------------------------
// Utilities
// -----------------------------
static std::string hex32(uint32_t v) {
    std::ostringstream oss;
    oss << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << v;
    return oss.str();
}

static std::string hex16(uint16_t v) {
    std::ostringstream oss;
    oss << "0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << v;
    return oss.str();
}

// Fetch 32-bit instruction from BIOS ROM region
static bool fetch32(uint32_t addr, uint32_t& out) {
    if (addr % 4 != 0) {
        lastError = "Misaligned PC fetch at " + hex32(addr);
        halted = true;
        return false;
    }
    if (addr < BIOS_BASE) {
        lastError = "Fetch below BIOS base at " + hex32(addr);
        halted = true;
        return false;
    }
    auto offset = addr - BIOS_BASE;
    if (offset + 4 > biosROM.size()) {
        lastError = "Fetch out of range (ROM size=" + std::to_string(biosROM.size()) + ") at " + hex32(addr);
        halted = true;
        return false;
    }
    out = (static_cast<uint32_t>(biosROM[offset]) << 24) |
          (static_cast<uint32_t>(biosROM[offset + 1]) << 16) |
          (static_cast<uint32_t>(biosROM[offset + 2]) << 8) |
          (static_cast<uint32_t>(biosROM[offset + 3]));
    return true;
}

// -----------------------------
// Decoder (minimal MIPS subset)
// -----------------------------
static std::string decode(uint32_t instr) {
    auto opcode = instr >> 26;
    auto rs = (instr >> 21) & 0x1F;
    auto rt = (instr >> 16) & 0x1F;
    auto rd = (instr >> 11) & 0x1F;
    auto imm = static_cast<uint16_t>(instr & 0xFFFF); // use auto with cast
    auto target = instr & 0x03FFFFFF;
    auto funct = instr & 0x3F;

    auto regN = [](uint32_t r) { return std::string("$") + std::to_string(r); };

    switch (opcode) {
        case 0x00: // SPECIAL
            switch (funct) {
                case 0x20: return "ADD " + regN(rd) + ", " + regN(rs) + ", " + regN(rt);
                case 0x22: return "SUB " + regN(rd) + ", " + regN(rs) + ", " + regN(rt);
                case 0x08: return "JR " + regN(rs);
                default:   return "UNKNOWN SPECIAL (" + std::to_string(funct) + ")";
            }
        case 0x0F: return "LUI " + regN(rt) + ", " + hex16(imm);
        case 0x08: return "ADDI " + regN(rt) + ", " + regN(rs) + ", " + hex16(imm);
        case 0x04: return "BEQ " + regN(rs) + ", " + regN(rt) + ", offset=" + hex16(imm);
        case 0x02: {
            auto absTarget = ((PC + 4) & 0xF0000000) | (target << 2);
            return "J " + hex32(absTarget);
        }
        default:   return "UNKNOWN OPC(" + std::to_string(opcode) + ")";
    }
}

// Execute minimal behavior for supported instructions
static bool execute(uint32_t instr) {
    auto opcode = instr >> 26;
    auto rs = (instr >> 21) & 0x1F;
    auto rt = (instr >> 16) & 0x1F;
    auto rd = (instr >> 11) & 0x1F;
    auto imm = static_cast<int16_t>(instr & 0xFFFF);
    auto target = instr & 0x03FFFFFF;
    auto funct = instr & 0x3F;

    auto nextPC = PC + 4;

    switch (opcode) {
        case 0x00: { // SPECIAL
            switch (funct) {
                case 0x20: // ADD
                    regs[rd] = regs[rs] + regs[rt];
                    PC = nextPC;
                    break;
                case 0x22: // SUB
                    regs[rd] = regs[rs] - regs[rt];
                    PC = nextPC;
                    break;
                case 0x08: { // JR
                    PC = regs[rs];
                    break;
                }
                default:
                    lastError = "Unknown SPECIAL funct=" + std::to_string(funct) + " at " + hex32(PC);
                    halted = true;
                    return false;
            }
            break;
        }
        case 0x0F: // LUI
            regs[rt] = (static_cast<uint32_t>(static_cast<uint16_t>(imm)) << 16);
            PC = nextPC;
            break;
        case 0x08: // ADDI
            regs[rt] = regs[rs] + imm;
            PC = nextPC;
            break;
        case 0x04: { // BEQ
            if (regs[rs] == regs[rt]) {
                // Branch offset is imm << 2, applied to nextPC
                PC = nextPC + (static_cast<int32_t>(imm) << 2);
            } else {
                PC = nextPC;
            }
            break;
        }
        case 0x02: { // J
            auto absTarget = (nextPC & 0xF0000000) | (target << 2);
            PC = absTarget;
            break;
        }
        default:
            lastError = "Unknown opcode=" + std::to_string(opcode) + " at " + hex32(PC);
            halted = true;
            return false;
    }

    // Enforce $zero = 0
    regs[0] = 0;
    return true;
}

// -----------------------------
// Public API
// -----------------------------
extern "C" {

// Initialize core
void initCore() {
    PC = BIOS_BASE;
    cycles = 0;
    biosLoaded = false;
    halted = false;
    lastError.clear();

    // ✅ Range-based loop with auto to match array element type
    for (auto& reg : regs) reg = 0;

    biosROM.clear();
    biosROM1.clear();
    biosROM2.clear();
    biosEROM.clear();
    biosNVM.clear();
    biosMEC.clear();
}

// Load BIOS parts (kind: "ROM","BIN","ROM1","ROM2","EROM","NVM","MEC")
bool loadBiosPart(const char* kind, const uint8_t* data, int length) {
    if (!kind || !data || length <= 0) return false;
    std::string k(kind);

    try {
        if (k == "ROM" || k == "BIN") {
            biosROM.assign(data, data + length);
            biosLoaded = !biosROM.empty();
        } else if (k == "ROM1") {
            biosROM1.assign(data, data + length);
        } else if (k == "ROM2") {
            biosROM2.assign(data, data + length);
        } else if (k == "EROM") {
            biosEROM.assign(data, data + length);
        } else if (k == "NVM") {
            biosNVM.assign(data, data + length);
        } else if (k == "MEC") {
            biosMEC.assign(data, data + length);
        } else {
            lastError = "Unknown BIOS kind: " + k;
            return false;
        }
    } catch (...) {
        lastError = "Failed to load BIOS part: " + k;
        return false;
    }
    return true;
}

// Step one instruction (real fetch/decode/execute)
void step() {
    if (!biosLoaded || halted) return;

    uint32_t instr = 0;
    if (!fetch32(PC, instr)) {
        // fetch32 sets halted + lastError
        return;
    }

    // Minimal execution
    auto ok = execute(instr);
    if (!ok) {
        // execute sets halted + lastError
        return;
    }

    // Increment cycles (approximate: 1 instruction = 4 cycles for v0.2)
    cycles += 4;
}

// Return a human-readable debug state
const char* getDebugState() {
    static std::string s;   // persistent storage
    static char* cstr = nullptr;

    // Prepare values
    uint32_t instr = 0;
    bool instrOk = (!halted && biosLoaded) ? fetch32(PC, instr) : false;
    std::string decoded = instrOk ? decode(instr)
                                  : (halted ? "HALTED" : (biosLoaded ? "FETCH ERROR" : "NO BIOS"));

    // Key registers to show quickly (adjust as you like)
    uint32_t ra = regs[31];
    uint32_t gp = regs[28];
    uint32_t k0 = regs[26];

    // BIOS status
    auto sizeStr = [](const std::vector<uint8_t>& v) {
        return v.empty() ? "0" : std::to_string(v.size());
    };

    std::ostringstream oss;
    oss << "PC=" << hex32(PC) << "  Cycles=" << cycles << "\n";
    if (instrOk) {
        oss << "Instr=" << hex32(instr) << "\n";
        oss << "Decoded=" << decoded << "\n";
    } else {
        oss << "Instr=(n/a)\n";
        oss << "Decoded=" << decoded << "\n";
    }
    oss << "Regs: $ra=" << hex32(ra) << "  $gp=" << hex32(gp) << "  $k0=" << hex32(k0) << "\n";
    oss << "BIOS: ROM=" << sizeStr(biosROM)
        << "  ROM1=" << sizeStr(biosROM1)
        << "  ROM2=" << sizeStr(biosROM2)
        << "  EROM=" << sizeStr(biosEROM)
        << "  NVM=" << sizeStr(biosNVM)
        << "  MEC=" << sizeStr(biosMEC) << "\n";
    if (halted && !lastError.empty()) {
        oss << "Status: HALTED (" << lastError << ")\n";
    } else if (!biosLoaded) {
        oss << "Status: NO BIOS LOADED\n";
    } else {
        oss << "Status: RUNNING\n";
    }

    s = oss.str();

    // Manage C-string lifetime for JNI-style returns
    if (cstr) { delete[] cstr; cstr = nullptr; }
    cstr = new char[s.size() + 1];
    std::memcpy(cstr, s.c_str(), s.size() + 1);
    return cstr;
}

// Convenience for UI readiness
bool isDebugReady() {
    return biosLoaded && !halted; // simplified expression
}

} // extern "C"