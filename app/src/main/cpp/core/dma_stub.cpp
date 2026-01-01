#include "dma_stub.h"
#include "mem_map.h"
#include "gs_stub.h"
#include <cstdint>
#include <cstring>

// Simple helpers for register mapping (addresses illustrative)
static inline bool isGifChannelActive(const DMAChannel& ch) {
    // CHCR STR bit (bit 8) indicates start/active
    return (ch.chcr & 0x100u) != 0;
}

void dmaInit(DMAC& dmac) {
    std::memset(&dmac, 0, sizeof(DMAC));
    // Default priorities, masks, etc. can be set here if needed
    // For now, keep it minimal.
}

// Returns number of GIF pushes (QWC processed)
uint32_t dmaStep(DMAC& dmac, Mem& mem, GS& gs) {
    uint32_t gifPushes = 0;

    // GIF channel is channel 2
    DMAChannel& ch2 = dmac.channels[2];

    if (isGifChannelActive(ch2)) {
        // Source pointer in RAM (wrap to RAM size)
        const size_t ramMask = mem.ram.size() - 1;
        const uint8_t* base  = mem.ram.data();
        const size_t   madr  = static_cast<size_t>(ch2.madr) & ramMask;

        const uint32_t* src = reinterpret_cast<const uint32_t*>(base + madr);
        int qwc = static_cast<int>(ch2.qwc);

        if (src && qwc > 0) {
            // Feed packet into GS
            gsProcessGifPacket(gs, src, qwc);
            gifPushes += static_cast<uint32_t>(qwc);

            // Advance DMA state: each QWC = 16 bytes (128 bits)
            ch2.madr += static_cast<uint32_t>(qwc * 16);
            ch2.qwc   = 0;
            ch2.chcr &= ~0x100u; // clear STR (stop)
        }
    }

    // You can add other channels here (VIF0/1, SIF, etc.) as needed.

    return gifPushes;
}

// Minimal register read (map a few core regs; expand as needed)
uint32_t dmaReadReg(const DMAC& dmac, uint32_t addr) {
    // Example mapping (addresses illustrative; align with your mem_map)
    // D_CTRL: 0x1000E000, D_STAT: 0x1000E010, D_PCR: 0x1000E020, D_SQWC: 0x1000E030
    switch (addr) {
        case 0x1000E000: return dmac.ctrl;
        case 0x1000E010: return dmac.stat;
        case 0x1000E020: return dmac.pcr;
        case 0x1000E030: return dmac.sqwc;
        default:
            // Channel CHCR/MADR/TADR/QWC reads could be added here
            return 0;
    }
}

// Minimal register write (update core regs; expand as needed)
void dmaWriteReg(DMAC& dmac, Mem& mem, uint32_t addr, uint32_t val) {
    (void)mem; // currently unused in this stub

    switch (addr) {
        case 0x1000E000: // D_CTRL
            dmac.ctrl = val;
            break;
        case 0x1000E010: // D_STAT
            dmac.stat = val;
            break;
        case 0x1000E020: // D_PCR
            dmac.pcr = val;
            break;
        case 0x1000E030: // D_SQWC
            dmac.sqwc = val;
            break;
        default:
            // Channel register writes (e.g., CHCR/MADR/TADR/QWC) should be handled
            // via your mem_map MMIO handlers that update dmac.channels[n].
            break;
    }
}