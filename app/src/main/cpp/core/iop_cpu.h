#pragma once
#include <cstdint>
#include "mem_map.h"
#include "cpu_common.h"

// IOP (Input/Output Processor) register state
struct IOPRegs {
    uint32_t pc = 0;      // Program counter
    uint32_t nextPc = 0;  // Next program counter
    // Add more IOP registers here as needed
};

// Initialize IOP state
inline void iopInit(IOPRegs& iop, uint32_t startPc) {
    iop.pc = startPc;
    iop.nextPc = startPc + 4;
}

// Execute one IOP instruction
ExecResult iopStep(IOPRegs& iop, Mem& mem, uint32_t opcode);