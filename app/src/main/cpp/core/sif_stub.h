#pragma once
#include <cstdint>

struct SIF {
    uint32_t main_ctrl = 0;
    uint32_t sub_ctrl = 0;
    uint32_t ms_com = 0;
    uint32_t sm_com = 0;
    // SIF DMA would go here
};

void sifInit(SIF& s);
uint32_t sifReadReg(SIF& s, uint32_t addr);
void sifWriteReg(SIF& s, uint32_t addr, uint32_t val);
