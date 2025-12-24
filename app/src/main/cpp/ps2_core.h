// ps2_core.h
#pragma once
#include <cstdint>

extern "C" {
void initCore();
bool loadBiosPart(const char* kind, const uint8_t* data, int length);
void step();
const char* getDebugState();
bool isDebugReady();
}