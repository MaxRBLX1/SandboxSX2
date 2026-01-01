#include "sif_stub.h"

void sifInit(SIF& s) {
    s = SIF{};
}

uint32_t sifReadReg(SIF& s, uint32_t addr) {
    switch (addr) {
        case 0x1000F230: return s.main_ctrl | 0x10001; // Force 'IOP Ready' bits
        case 0x1D000010: return s.sub_ctrl;
        case 0x1000F210: return s.ms_com; // EE to IOP
        case 0x1000F220: return s.sm_com; // IOP to EE
        default: return 0;
    }
}

void sifWriteReg(SIF& s, uint32_t addr, uint32_t val) {
    switch (addr) {
        case 0x1000F230: s.main_ctrl = val; break;
        case 0x1D000010: s.sub_ctrl = val; break;
        case 0x1000F210: s.ms_com = val; break;
        case 0x1000F220: s.sm_com = val; break;
        default: break;
    }
}
