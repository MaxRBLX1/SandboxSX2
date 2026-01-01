// ps2_core.cpp
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring> // std::memcpy
#include <algorithm> // std::fill
#include "ps2_core.h"

// -----------------------------
// Configuration and state
// -----------------------------
static const uint32_t BIOS_BASE = 0xBFC00000;

// EE memory regions (v0.3)
static const uint32_t EERAM_BASE   = 0x00000000; // 0x00000000–0x01FFFFFF (32 MB)
static const uint32_t EERAM_SIZE   = 32 * 1024 * 1024;
static const uint32_t SCRATCH_BASE = 0x70000000; // 0x70000000–0x70003FFF (16 KB)
static const uint32_t SCRATCH_SIZE = 16 * 1024;
static const uint32_t IOPRAM_BASE  = 0x1C000000; // 0x1C000000–0x1C7FFFFF (8 MB)
static const uint32_t IOPRAM_SIZE  = 8 * 1024 * 1024;

static uint32_t PC = BIOS_BASE;            // Program Counter starts at BIOS entry
static uint64_t cycles = 0;                // Cycle counter
static bool biosLoaded = false;            // True when main ROM is present
static bool coreInitialized = false;       // True after initCore and ROM presence check
static bool halted = false;                // Halt on errors
static std::string lastError;              // Last error message (for debug)

static uint32_t regs[32] = {0};            // EE GPRs (simplified)

// BIOS parts (execute primarily from ROM for now)
static std::vector<uint8_t> biosROM;       // Main BIOS image
static std::vector<uint8_t> biosROM1;      // Auxiliary BIOS 1
static std::vector<uint8_t> biosROM2;      // Auxiliary BIOS 2
static std::vector<uint8_t> biosEROM;      // Extended ROM
static std::vector<uint8_t> biosNVM;       // NVM (not executable)
static std::vector<uint8_t> biosMEC;       // MEC (not executable)

// EE memory buffers (v0.3)
static std::vector<uint8_t> eeRAM(EERAM_SIZE, 0);
static std::vector<uint8_t> scratchpad(SCRATCH_SIZE, 0);
static std::vector<uint8_t> iopRAM(IOPRAM_SIZE, 0);

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

static inline bool inRange(uint32_t addr, uint32_t base, uint32_t size) {
    return addr >= base && addr < (base + size);
}

// Fetch 32-bit instruction from BIOS ROM region (big-endian in ROM storage)
static bool fetch32(uint32_t addr, uint32_t& out) {
    if (!biosLoaded) {
        lastError = "Fetch without BIOS loaded";
        halted = true;
        return false;
    }
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
    const uint32_t offset = addr - BIOS_BASE;
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
// Memory access helpers (v0.3)
// -----------------------------
static bool load32(uint32_t addr, uint32_t& out) {
    if (addr % 4 != 0) {
        lastError = "Misaligned load32 at " + hex32(addr);
        halted = true;
        return false;
    }

    if (inRange(addr, EERAM_BASE, EERAM_SIZE)) {
        const uint32_t off = addr - EERAM_BASE;
        out = (static_cast<uint32_t>(eeRAM[off]) << 24) |
              (static_cast<uint32_t>(eeRAM[off + 1]) << 16) |
              (static_cast<uint32_t>(eeRAM[off + 2]) << 8) |
              (static_cast<uint32_t>(eeRAM[off + 3]));
        return true;
    }
    if (inRange(addr, SCRATCH_BASE, SCRATCH_SIZE)) {
        const uint32_t off = addr - SCRATCH_BASE;
        out = (static_cast<uint32_t>(scratchpad[off]) << 24) |
              (static_cast<uint32_t>(scratchpad[off + 1]) << 16) |
              (static_cast<uint32_t>(scratchpad[off + 2]) << 8) |
              (static_cast<uint32_t>(scratchpad[off + 3]));
        return true;
    }
    if (inRange(addr, IOPRAM_BASE, IOPRAM_SIZE)) {
        const uint32_t off = addr - IOPRAM_BASE;
        out = (static_cast<uint32_t>(iopRAM[off]) << 24) |
              (static_cast<uint32_t>(iopRAM[off + 1]) << 16) |
              (static_cast<uint32_t>(iopRAM[off + 2]) << 8) |
              (static_cast<uint32_t>(iopRAM[off + 3]));
        return true;
    }

    lastError = "load32 unmapped addr " + hex32(addr);
    halted = true;
    return false;
}

static bool store32(uint32_t addr, uint32_t value) {
    if (addr % 4 != 0) {
        lastError = "Misaligned store32 at " + hex32(addr);
        halted = true;
        return false;
    }

    auto b0 = static_cast<uint8_t>(value >> 24);
    auto b1 = static_cast<uint8_t>(value >> 16);
    auto b2 = static_cast<uint8_t>(value >> 8);
    auto b3 = static_cast<uint8_t>(value);

    if (inRange(addr, EERAM_BASE, EERAM_SIZE)) {
        const uint32_t off = addr - EERAM_BASE;
        eeRAM[off]     = b0;
        eeRAM[off + 1] = b1;
        eeRAM[off + 2] = b2;
        eeRAM[off + 3] = b3;
        return true;
    }
    if (inRange(addr, SCRATCH_BASE, SCRATCH_SIZE)) {
        const uint32_t off = addr - SCRATCH_BASE;
        scratchpad[off]     = b0;
        scratchpad[off + 1] = b1;
        scratchpad[off + 2] = b2;
        scratchpad[off + 3] = b3;
        return true;
    }
    if (inRange(addr, IOPRAM_BASE, IOPRAM_SIZE)) {
        const uint32_t off = addr - IOPRAM_BASE;
        iopRAM[off]     = b0;
        iopRAM[off + 1] = b1;
        iopRAM[off + 2] = b2;
        iopRAM[off + 3] = b3;
        return true;
    }

    lastError = "store32 unmapped addr " + hex32(addr);
    halted = true;
    return false;
}

// -----------------------------
// Decoder (minimal MIPS subset)
// -----------------------------
static std::string decode(uint32_t instr) {
    const uint32_t opcode = instr >> 26;
    const uint32_t rs = (instr >> 21) & 0x1F;
    const uint32_t rt = (instr >> 16) & 0x1F;
    const uint32_t rd = (instr >> 11) & 0x1F;
    const auto imm = static_cast<uint16_t>(instr & 0xFFFF);
    const uint32_t target = instr & 0x03FFFFFF;
    const uint32_t funct = instr & 0x3F;
    const uint32_t shamt = (instr >> 6) & 0x1F;

    auto regN = [](uint32_t r) { return std::string("$") + std::to_string(r); };

    switch (opcode) {
        case 0x00: // SPECIAL
            switch (funct) {
                case 0x00: return "SLL " + regN(rd) + ", " + regN(rt) + ", " + std::to_string(shamt); // NOP if all zero
                case 0x20: return "ADD " + regN(rd) + ", " + regN(rs) + ", " + regN(rt);
                case 0x22: return "SUB " + regN(rd) + ", " + regN(rs) + ", " + regN(rt);
                case 0x08: return "JR " + regN(rs);
                default:   return "UNKNOWN SPECIAL (" + std::to_string(funct) + ")";
            }
        case 0x0F: return "LUI " + regN(rt) + ", " + hex16(imm);
        case 0x08: return "ADDI " + regN(rt) + ", " + regN(rs) + ", " + hex16(imm);
        case 0x04: return "BEQ " + regN(rs) + ", " + regN(rt) + ", offset=" + hex16(imm);
        case 0x23: return "LW " + regN(rt) + ", " + hex16(imm) + "(" + regN(rs) + ")";
        case 0x2B: return "SW " + regN(rt) + ", " + hex16(imm) + "(" + regN(rs) + ")";
        case 0x02: {
            const uint32_t absTarget = ((PC + 4) & 0xF0000000) | (target << 2);
            return "J " + hex32(absTarget);
        }
        default:   return "UNKNOWN OPC(" + std::to_string(opcode) + ")";
    }
}

// Execute minimal behavior for supported instructions
static bool execute(uint32_t instr) {
    const uint32_t opcode = instr >> 26;
    const uint32_t rs = (instr >> 21) & 0x1F;
    const uint32_t rt = (instr >> 16) & 0x1F;
    const uint32_t rd = (instr >> 11) & 0x1F;
    const auto imm = static_cast<int16_t>(instr & 0xFFFF);
    const uint32_t target = instr & 0x03FFFFFF;
    const uint32_t funct = instr & 0x3F;
    const uint32_t shamt = (instr >> 6) & 0x1F;

    const uint32_t nextPC = PC + 4;

    switch (opcode) {
        case 0x00: { // SPECIAL
            switch (funct) {
                case 0x00: { // SLL (NOP if rt=0, rd=0, shamt=0)
                    if (rt == 0 && rd == 0 && shamt == 0) {
                        PC = nextPC; // NOP
                    } else {
                        regs[rd] = regs[rt] << shamt;
                        PC = nextPC;
                    }
                    break;
                }
                case 0x20: // ADD
                    regs[rd] = regs[rs] + regs[rt];
                    PC = nextPC;
                    break;
                case 0x22: // SUB
                    regs[rd] = regs[rs] - regs[rt];
                    PC = nextPC;
                    break;
                case 0x08: // JR
                    PC = regs[rs];
                    break;
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
                PC = nextPC + (static_cast<int32_t>(imm) << 2);
            } else {
                PC = nextPC;
            }
            break;
        }
        case 0x23: { // LW
            const auto addr = static_cast<uint32_t>(regs[rs] + imm);
            uint32_t val = 0;
            if (!load32(addr, val)) return false;
            regs[rt] = val;
            PC = nextPC;
            break;
        }
        case 0x2B: { // SW
            const auto addr = static_cast<uint32_t>(regs[rs] + imm);
            if (!store32(addr, regs[rt])) return false;
            PC = nextPC;
            break;
        }
        case 0x02: { // J
            const uint32_t absTarget = (nextPC & 0xF0000000) | (target << 2);
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

// Initialize core (used by Init Core button)
void initCore() {
    PC = BIOS_BASE;
    cycles = 0;
    halted = false;
    lastError.clear();
    coreInitialized = false;  // will flip true only if ROM present

    for (auto &reg : regs) reg = 0;

    // Clear EE memory (keep BIOS buffers as-is)
    std::fill(eeRAM.begin(), eeRAM.end(), 0);
    std::fill(scratchpad.begin(), scratchpad.end(), 0);
    std::fill(iopRAM.begin(), iopRAM.end(), 0);

    // Core becomes initialized only if ROM is already loaded
    if (!biosROM.empty()) {
        biosLoaded = true;
        coreInitialized = true;
    } else {
        biosLoaded = false;
        coreInitialized = false;
    }
}

// Load BIOS parts (kind: "ROM","BIN","ROM1","ROM2","EROM","NVM","MEC")
bool loadBiosPart(const char* kind, const uint8_t* data, int length) {
    if (!kind || !data || length <= 0) return false;
    const std::string k(kind);

    try {
        if (k == "ROM" || k == "BIN") {
            biosROM.assign(data, data + length);
            biosLoaded = !biosROM.empty();
            // If core was previously initialized, keep it initialized
            // Otherwise, leave initCore to set flags properly.
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

// Step one instruction (fetch/decode/execute)
void step() {
    // v0.3: only step if ROM loaded and core initialized and not halted
    if (!biosLoaded || !coreInitialized || halted) return;

    uint32_t instr = 0;
    if (!fetch32(PC, instr)) {
        // fetch32 sets halted + lastError
        return;
    }

    const bool ok = execute(instr);
    if (!ok) {
        // execute sets halted + lastError
        return;
    }

    // Approximate cycles (placeholder)
    cycles += 4;
}

// Return a human-readable debug state
const char* getDebugState() {
    static std::string s;    // persistent storage
    static char* cstr = nullptr;

    // Prepare values
    uint32_t instr = 0;
    bool instrOk = (!halted && biosLoaded && coreInitialized) && fetch32(PC, instr);
    std::string decoded = instrOk ? decode(instr)
                                  : (halted ? "HALTED"
                                            : (!biosLoaded ? "NO BIOS"
                                                           : (!coreInitialized ? "CORE NOT INITIALIZED"
                                                                               : "FETCH ERROR")));

    // Sample registers
    const uint32_t ra = regs[31];
    const uint32_t gp = regs[28];
    const uint32_t k0 = regs[26];

    // Vector size to string
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
    oss << "Mem: EE=" << eeRAM.size()
        << "  Scratch=" << scratchpad.size()
        << "  IOP=" << iopRAM.size() << "\n";

    if (halted && !lastError.empty()) {
        oss << "Status: HALTED (" << lastError << ")\n";
    } else if (!biosLoaded) {
        oss << "Status: NO BIOS LOADED\n";
    } else if (!coreInitialized) {
        oss << "Status: CORE NOT INITIALIZED\n";
    } else {
        oss << "Status: READY\n";
    }

    s = oss.str();

    // Manage C-string lifetime for JNI-friendly returns
    if (cstr) { delete[] cstr; cstr = nullptr; }
    cstr = new char[s.size() + 1];
    std::memcpy(cstr, s.c_str(), s.size() + 1);
    return cstr;
}

// UI readiness: require ROM loaded, core initialized, and not halted
bool isDebugReady() {
    return biosLoaded && coreInitialized && !halted;
}

} // extern "C"