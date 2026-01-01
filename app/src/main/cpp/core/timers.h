#pragma once
#include <cstdint>

// PS2 Hardware Timer (T0..T3)
struct Ps2Timer {
    uint32_t count = 0;
    uint32_t mode = 0;
    uint32_t target = 0;
    [[maybe_unused]] bool running = false;
};

struct Timers {
    Ps2Timer t[4]; // T0, T1, T2, T3
};

void timersInit(Timers& timers);
void timersStep(Timers& timers, uint32_t cycles, uint32_t& intc_stat); // Updates INTC directly
uint32_t timerRead(Timers& timers, uint32_t addr);
void timerWrite(Timers& timers, uint32_t addr, uint32_t val);
