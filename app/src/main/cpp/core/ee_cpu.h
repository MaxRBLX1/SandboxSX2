#pragma once
#include <cstdint>
#include "mem_map.h"
#include "cpu_common.h"

// Emotion Engine (EE) register state
struct EERegs {
    uint32_t GPR[32] = {0}; // General-purpose registers r0..r31
    uint32_t HI = 0;        // Multiply/divide high result
    uint32_t LO = 0;        // Multiply/divide low result
    uint32_t pc = 0;        // Program counter
    uint32_t nextPc = 0;    // Next program counter (branch delay slot)
};

// Initialize EE state
inline void eeInit(EERegs& ee, uint32_t startPc) {
    for (int i = 0; i < 32; ++i) ee.GPR[i] = 0;
    ee.HI = ee.LO = 0;
    ee.pc = startPc;
    ee.nextPc = startPc + 4;
}

// Execute one EE instruction
ExecResult eeStep(EERegs& ee, Mem& mem, uint32_t opcode);

// Register helpers
inline uint32_t eeGetReg(const EERegs& ee, int idx) {
    return (idx & 31) == 0 ? 0u : ee.GPR[idx & 31];
}

inline void eeSetReg(EERegs& ee, int idx, uint32_t val) {
    if ((idx & 31) != 0) ee.GPR[idx & 31] = val;
}