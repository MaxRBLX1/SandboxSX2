#include "ps2_core.h"

#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <cstdint>
#include <cstdio>

// BIOS storage
static std::vector<uint8_t> g_biosData;
static std::mutex g_biosLock;

// VM state
static std::atomic<long long> g_tickCount{0};
static uint32_t g_pc = 0xBFC00000; // PS2 reset vector
static std::string g_debugState = "EE loop initialized";

// Simple register file (stubbed)
static uint32_t g_registers[32] = {0};

// --- Timing & interrupts (stubbed) ---
static std::atomic<uint64_t> g_cycles{0};      // EE cycles (conceptual)
static std::atomic<uint64_t> g_timer0{0};      // simple timer counter
static std::atomic<bool>     g_irqPending{false}; // fake interrupt flag

// BIOS base constant
static constexpr uint32_t BIOS_BASE = 0xBFC00000;

// --- Fetch ---
static inline uint32_t read_u32_le(const uint8_t* p) {
    // Treat BIOS bytes as little-endian for simplicity; adjust if needed
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint32_t fetchInstruction(uint32_t pc) {
    std::lock_guard<std::mutex> lock(g_biosLock);
    if (g_biosData.empty()) return 0;

    if (pc < BIOS_BASE) return 0;
    size_t offset = static_cast<size_t>(pc - BIOS_BASE);
    if (offset + 4 <= g_biosData.size()) {
        return read_u32_le(&g_biosData[offset]);
    }
    return 0; // NOP if out of bounds
}

// --- Decode + Execute (intentionally stubbed) ---
static void decodeAndExecute(uint32_t opcode) {
    // Classification-only stub: side effects allowed to be no-ops
    if (opcode == 0x00000000) { // NOP
        g_debugState = "NOP executed";
        return;
    }

    // Minimal SPECIAL/J-type classification (no semantics yet)
    uint32_t op = (opcode >> 26) & 0x3F;
    switch (op) {
        case 0x00: { // SPECIAL
            uint32_t funct = opcode & 0x3F;
            g_debugState = "SPECIAL funct=0x" + std::to_string(funct);
            break;
        }
        case 0x02: { // J
            g_debugState = "J-type opcode";
            break;
        }
        case 0x03: { // JAL
            g_debugState = "JAL opcode";
            break;
        }
        default: {
            g_debugState = "Opcode: 0x" + std::to_string(opcode);
            break;
        }
    }
    // Side effects are intentionally minimal/no-op for v0.5
}

// --- Synchronize (the most important conceptual phase) ---
static void synchronize() {
    // Advance virtual cycles—this is where "time moves forward"
    g_cycles += 1;

    // Simple timer: increment every tick; trigger a fake IRQ periodically
    g_timer0 += 1;

    // Example: raise an interrupt every 4096 cycles
    if ((g_cycles.load() & 0xFFF) == 0) {
        g_irqPending.store(true);
    }

    // Handle pending interrupt (stubbed)
    if (g_irqPending.load()) {
        g_irqPending.store(false);
        g_debugState = "IRQ serviced | cycles=" + std::to_string(g_cycles.load());
    }

    // Future hooks:
    // - DMA scheduling & progress
    // - VU co-processor sync
    // - GS timing (no rendering yet)
    // - SPU2 audio timing
}

// --- Internal API (called from ps2_jni.cpp) ---
bool ps2core_loadBiosPart(JNIEnv* env, jstring part, jbyteArray bytes) {
    const char* partStr = env->GetStringUTFChars(part, nullptr);
    jsize length = env->GetArrayLength(bytes);
    jbyte* data = env->GetByteArrayElements(bytes, nullptr);

    if (length <= 0 || data == nullptr) {
        if (data) env->ReleaseByteArrayElements(bytes, data, JNI_ABORT);
        env->ReleaseStringUTFChars(part, partStr);
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(g_biosLock);
        g_biosData.assign(reinterpret_cast<uint8_t*>(data),
                          reinterpret_cast<uint8_t*>(data) + length);
    }

    env->ReleaseByteArrayElements(bytes, data, JNI_ABORT);
    env->ReleaseStringUTFChars(part, partStr);
    return true;
}

void ps2core_tick() {
    // Stage 1: Fetch
    uint32_t opcode = fetchInstruction(g_pc);

    // Stage 2 & 3: Decode + Execute (stubbed semantics)
    if (!g_biosData.empty()) {
        decodeAndExecute(opcode);
        // Advance PC by one instruction (4 bytes)
        g_pc += 4;
    } else {
        g_debugState = "BIOS not loaded";
    }

    // Stage 4: Synchronize (advance time, timers, interrupts)
    synchronize();

    // Stage 5: Repeat is driven externally by Kotlin coroutine
    g_tickCount++;

    // Update debug summary (hex PC for readability)
    char buf[128];
    std::snprintf(buf, sizeof(buf), "Tick %lld | PC=0x%08X | cycles=%llu",
                  g_tickCount.load(), g_pc, (unsigned long long)g_cycles.load());
    g_debugState = buf;
}

uint32_t ps2core_getPC() {
    return g_pc;
}

long long ps2core_getTickCount() {
    return g_tickCount.load();
}

jstring ps2core_getDebugState(JNIEnv* env) {
    return env->NewStringUTF(g_debugState.c_str());
}