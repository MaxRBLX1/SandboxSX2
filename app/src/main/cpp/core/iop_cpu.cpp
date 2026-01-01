#include "iop_cpu.h"
#include "mem_map.h"

// Minimal stub implementation of the IOP CPU step.
// This keeps the linker happy and allows the emulator to run,
// even though real IOP instruction decoding is not yet implemented.

ExecResult iopStep(IOPRegs& iop, Mem& mem, uint32_t opcode) {
    // For now, just advance the PC by 4 bytes (next instruction).
    iop.pc += 4;
    iop.nextPc = iop.pc + 4;

    // Later, expand this to decode and execute real IOP opcodes.
    return ExecResult::Ok;
}