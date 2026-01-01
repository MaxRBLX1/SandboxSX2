#include "debug_bus.h"
#include <deque>
#include <vector>

static std::deque<std::string> gDbgQueue;

void dbgPush(const std::string& msg) {
    gDbgQueue.push_back(msg);
    if (gDbgQueue.size() > 100) {
        gDbgQueue.pop_front(); // keep memory bounded
    }
}

const std::string& dbgLast() {
    static std::string empty;
    return gDbgQueue.empty() ? empty : gDbgQueue.back();
}

void dbgClear() {
    gDbgQueue.clear();
}

std::vector<std::string> dbgDump() {
    return std::vector<std::string>(gDbgQueue.begin(), gDbgQueue.end());
}