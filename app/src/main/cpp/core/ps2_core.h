// ps2_core.h
#pragma once
#include <jni.h>
#include <cstdint>

bool     ps2core_loadBiosPart(JNIEnv* env, jstring part, jbyteArray bytes);
void     ps2core_tick();
uint32_t ps2core_getPC();
long long ps2core_getTickCount();
jstring  ps2core_getDebugState(JNIEnv* env);
