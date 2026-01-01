#include "scheduler.h"

void schedInit(Scheduler& s) {
    s.ticks = 0;
}

void schedTick(Scheduler& s) {
    s.ticks++;
}
