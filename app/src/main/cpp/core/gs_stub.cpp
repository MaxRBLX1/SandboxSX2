// gs_stub.cpp
#include "gs_stub.h"
#include <cstdint>
#include <string>
#include <atomic>
#include <mutex>

// -----------------------------------------------------------------------------
// Minimal GS register + GIF packet stub
// -----------------------------------------------------------------------------

// Simple register bank (placeholder)
static uint32_t g_gsRegs[256] = {0};
static std::mutex g_gsLock;

// Debug state string
static std::string g_gsDebug = "GS stub initialized";

// -----------------------------------------------------------------------------
// Plain C symbol expected by linker (called from Kotlin/Java)
// -----------------------------------------------------------------------------

extern "C" {

// This satisfies the linker error: "undefined symbol: Native_setGsReg"
void Native_setGsReg(int index, int value) {
    std::lock_guard<std::mutex> lock(g_gsLock);

    if (index >= 0 && index < (int)(sizeof(g_gsRegs) / sizeof(g_gsRegs[0]))) {
        g_gsRegs[index] = (uint32_t)value;
        g_gsDebug = "GS reg[" + std::to_string(index) + "] = " + std::to_string(value);
    } else {
        g_gsDebug = "GS reg write out of bounds";
    }
}

} // extern "C"

// -----------------------------------------------------------------------------
// Internal helpers (future expansion)
// -----------------------------------------------------------------------------

// Read back a GS register
uint32_t gs_getReg(int index) {
    std::lock_guard<std::mutex> lock(g_gsLock);
    if (index >= 0 && index < (int)(sizeof(g_gsRegs) / sizeof(g_gsRegs[0]))) {
        return g_gsRegs[index];
    }
    return 0;
}

// Get debug string
const char* gs_getDebug() {
    return g_gsDebug.c_str();
}

// -----------------------------------------------------------------------------
// GIF packet processor stub (called from dma_stub.cpp)
// -----------------------------------------------------------------------------

void gsProcessGifPacket(GS& gs, const uint32_t* data, int qwc) {
    std::lock_guard<std::mutex> lock(g_gsLock);

    // Stub: just log the call
    g_gsDebug = "GIF packet received | qwc=" + std::to_string(qwc);

    // TODO (v0.6+): parse GIF tags, route to GS registers, simulate drawing
    // For now, this is a no-op placeholder
}