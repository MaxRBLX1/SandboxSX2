#pragma once
#include <cstdint>
#include <array>

struct Mem; // forward declaration
struct GS;  // forward declaration

struct DMAChannel {
    uint32_t madr = 0; // Memory Address
    uint32_t tadr = 0; // Tag Address
    uint32_t qwc  = 0; // Quadword Count
    uint32_t chcr = 0; // Control Register
};

struct DMAC {
    std::array<DMAChannel, 10> channels{}; // 10 DMA channels
    uint32_t ctrl = 0;   // D_CTRL
    uint32_t stat = 0;   // D_STAT
    uint32_t pcr  = 0;   // D_PCR (Priority Control)
    uint32_t sqwc = 0;   // D_SQWC (Skip Quadword Count)
};

// Function declarations
void     dmaInit(DMAC& dmac);
uint32_t dmaStep(DMAC& dmac, Mem& mem, GS& gs);  // returns number of GIF pushes
uint32_t dmaReadReg(const DMAC& dmac, uint32_t addr);
void     dmaWriteReg(DMAC& dmac, Mem& mem, uint32_t addr, uint32_t val);