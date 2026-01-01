#include "ee_cpu.h"
#include <cstdio>

// Field extractors
static inline uint32_t OPC(uint32_t op) { return (op >> 26) & 0x3F; }
static inline uint32_t RS (uint32_t op) { return (op >> 21) & 0x1F; }
static inline uint32_t RT (uint32_t op) { return (op >> 16) & 0x1F; }
static inline uint32_t RD (uint32_t op) { return (op >> 11) & 0x1F; }
static inline uint32_t SA (uint32_t op) { return (op >> 6)  & 0x1F; }
static inline uint32_t FUN(uint32_t op) { return  op        & 0x3F; }
static inline uint32_t IMM(uint32_t op) { return  op        & 0xFFFF; }
static inline uint32_t TGT(uint32_t op) { return  op        & 0x03FFFFFF; }

static inline int32_t  S16(uint32_t imm) { return static_cast<int16_t>(imm); }
static inline uint32_t Z16(uint32_t imm) { return imm & 0xFFFFu; }

// Memory helpers
static inline bool eeLoad32(Mem& mem, uint32_t addr, uint32_t& out) {
    if ((addr & 3u) != 0) return false;
    out = memRead32(mem, addr);
    return true;
}
static inline bool eeStore32(Mem& mem, uint32_t addr, uint32_t val) {
    if ((addr & 3u) != 0) return false;
    memWrite32(mem, addr, val);
    return true;
}

// Execute one instruction
static ExecResult eeExecOne(EERegs& ee, Mem& mem, uint32_t opcode,
                            bool& branchTaken, uint32_t& branchTarget) {
    ee.nextPc = ee.pc + 4;

    const uint32_t op  = OPC(opcode);
    const uint32_t rs  = RS(opcode);
    const uint32_t rt  = RT(opcode);
    const uint32_t rd  = RD(opcode);
    const uint32_t sa  = SA(opcode);
    const uint32_t fun = FUN(opcode);
    const uint32_t imm = IMM(opcode);
    const uint32_t tgt = TGT(opcode);

    const uint32_t vrs = eeGetReg(ee, rs);
    const uint32_t vrt = eeGetReg(ee, rt);

    branchTaken = false;
    branchTarget = 0;

    switch (op) {
        case 0x00: { // SPECIAL
            switch (fun) {
                case 0x00: // SLL (NOP if rd=0, rt=0, sa=0)
                    if (!(rd == 0 && rt == 0 && sa == 0))
                        eeSetReg(ee, rd, vrt << sa);
                    break;
                case 0x02: eeSetReg(ee, rd, vrt >> sa); break; // SRL
                case 0x03: eeSetReg(ee, rd, static_cast<int32_t>(vrt) >> sa); break; // SRA
                case 0x08: branchTaken = true; branchTarget = vrs; break; // JR
                case 0x09: eeSetReg(ee, rd ? rd : 31, ee.pc + 8);
                    branchTaken = true; branchTarget = vrs; break; // JALR
                case 0x23: eeSetReg(ee, rd, vrs - vrt); break; // SUBU
                case 0x24: eeSetReg(ee, rd, vrs & vrt); break; // AND
                case 0x25: eeSetReg(ee, rd, vrs | vrt); break; // OR
                case 0x26: eeSetReg(ee, rd, vrs ^ vrt); break; // XOR
                case 0x27: eeSetReg(ee, rd, ~(vrs | vrt)); break; // NOR
                case 0x2A: eeSetReg(ee, rd, (int32_t)vrs < (int32_t)vrt); break; // SLT
                case 0x2B: eeSetReg(ee, rd, vrs < vrt); break; // SLTU
                default: break;
            }
            break;
        }

        case 0x02: branchTaken = true;
            branchTarget = (ee.pc & 0xF0000000u) | (tgt << 2); break; // J
        case 0x03: eeSetReg(ee, 31, ee.pc + 8);
            branchTaken = true;
            branchTarget = (ee.pc & 0xF0000000u) | (tgt << 2); break; // JAL
        case 0x04: if (vrs == vrt) { branchTaken = true;
                branchTarget = ee.pc + 4 + (S16(imm) << 2); } break; // BEQ
        case 0x05: if (vrs != vrt) { branchTaken = true;
                branchTarget = ee.pc + 4 + (S16(imm) << 2); } break; // BNE

        case 0x09: eeSetReg(ee, rt, vrs + static_cast<uint32_t>(S16(imm))); break; // ADDIU
        case 0x0D: eeSetReg(ee, rt, vrs | Z16(imm)); break; // ORI
        case 0x0F: eeSetReg(ee, rt, Z16(imm) << 16); break; // LUI

        case 0x23: { // LW
            uint32_t addr = vrs + static_cast<uint32_t>(S16(imm));
            uint32_t val  = 0;
            if (!eeLoad32(mem, addr, val)) return ExecResult::Exception;
            eeSetReg(ee, rt, val);
            break;
        }
        case 0x2B: { // SW
            uint32_t addr = vrs + static_cast<uint32_t>(S16(imm));
            if (!eeStore32(mem, addr, vrt)) return ExecResult::Exception;
            break;
        }

        default: break;
    }

    return ExecResult::Ok;
}

ExecResult eeStep(EERegs& ee, Mem& mem, uint32_t opcode) {
    bool branchTaken = false;
    uint32_t branchTarget = 0;

    ExecResult r = eeExecOne(ee, mem, opcode, branchTaken, branchTarget);
    if (r == ExecResult::Exception) return r;

    if (branchTaken) {
        // Execute delay slot at pc+4
        uint32_t delayOp = memRead32(mem, ee.pc + 4);
        uint32_t savedPc = ee.pc;
        ee.pc = savedPc + 4;
        bool dummyTaken = false;
        uint32_t dummyTarget = 0;
        r = eeExecOne(ee, mem, delayOp, dummyTaken, dummyTarget);
        if (r == ExecResult::Exception) return r;

        // Commit branch target
        ee.pc = branchTarget;
        ee.nextPc = branchTarget + 4;
    } else {
        ee.pc = ee.pc + 4;
        ee.nextPc = ee.pc + 4;
    }

    return ExecResult::Ok;
}