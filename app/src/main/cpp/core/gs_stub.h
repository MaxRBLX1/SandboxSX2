#pragma once
#include <cstdint>
#include <vector>

enum class GSPrim : uint8_t {
    None = 0,
    Point = 1,
    Line = 2,
    Triangle = 3
};

struct GS {
    int width  = 0;
    int height = 0;
    uint64_t tick = 0;

    // Core GS registers (AD writes)
    uint64_t FRAME = 0;
    uint64_t ZBUF  = 0;
    uint64_t TEST  = 0;
    uint64_t ALPHA = 0;
    uint64_t TEX0  = 0;
    uint64_t TEX1  = 0;
    uint64_t PRIM  = 0;
    uint64_t RGBAQ = 0;
    uint64_t XYZ2  = 0;

    // Framebuffer (ARGB8888)
    std::vector<uint32_t> fb;

    // Primitive state
    GSPrim prim = GSPrim::None;
    uint32_t currentColor = 0xFFFFFFFF; // ARGB
    int vx[3] = {0,0,0};
    int vy[3] = {0,0,0};
    int vcount = 0;
};

// API
void gsInit(GS& gs, int w, int h);
void gsStep(GS& gs, uint64_t tick);
void gsProcessGifPacket(GS& gs, const uint32_t* data, int qwc);
const uint32_t* gsData(const GS& gs);