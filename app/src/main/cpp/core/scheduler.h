#pragma once
#include <cstdint>

struct Scheduler {
    [[maybe_unused]]    uint64_t ticks = 0;
};

void schedInit(Scheduler& s);
void schedTick(Scheduler& s);