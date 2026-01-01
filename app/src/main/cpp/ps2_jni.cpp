// ps2_jni.cpp
#include <jni.h>
<<<<<<< HEAD
#include <cstdint>
#include "ps2_core.h"

// Optional local GS register stub (replace with gs_stub.cpp calls later)
namespace {
    static uint32_t g_gsRegs[256] = {0}; // simple placeholder register bank
}

// NOTE: JNI naming rules:
// - Package: com.maxrblx1.sandboxsx2
// - Class:   NativeEmulator
// - Method:  Native_setGsReg  -> underscore becomes _1 in C symbol
//   => Java_com_maxrblx1_sandboxsx2_NativeEmulator_Native_1setGsReg

extern "C" {

// ----------------------------- Core VM glue -----------------------------

JNIEXPORT jboolean JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_nativeLoadBiosPart(JNIEnv* env, jobject thiz, jstring part, jbyteArray bytes) {
    return ps2core_loadBiosPart(env, part, bytes);
}

JNIEXPORT void JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_nativeTick(JNIEnv* env, jobject thiz) {
    ps2core_tick();
}

JNIEXPORT jint JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_nativeGetPC(JNIEnv* env, jobject thiz) {
    return (jint)ps2core_getPC();
}

JNIEXPORT jlong JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_nativeGetTickCount(JNIEnv* env, jobject thiz) {
    return (jlong)ps2core_getTickCount();
}

JNIEXPORT jstring JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_nativeGetDebugState(JNIEnv* env, jobject thiz) {
    return ps2core_getDebugState(env);
}

// ----------------------------- GS register stub -----------------------------

// Kotlin/Java declaration should be:
// external fun Native_setGsReg(index: Int, value: Int)
//
// JNI symbol mapping:
// Java_com_maxrblx1_sandboxsx2_NativeEmulator_Native_1setGsReg
JNIEXPORT void JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_Native_1setGsReg(JNIEnv* env, jobject thiz, jint index, jint value) {
    // Bounds-check and store in local stub array
    if (index >= 0 && index < (jint)(sizeof(g_gsRegs) / sizeof(g_gsRegs[0]))) {
        g_gsRegs[index] = (uint32_t)value;
    }
    // If you later add real GS handling:
    // gs_set_register((uint32_t)index, (uint32_t)value);
=======
#include <string>
#include "ps2_core.h"   // include your core directly (or better: ps2_core.h if you split headers)

// Helper: convert jstring -> std::string
static std::string jstringToStdString(JNIEnv* env, jstring jStr) {
    if (!jStr) return "";
    const char* chars = env->GetStringUTFChars(jStr, nullptr);
    std::string str(chars);
    env->ReleaseStringUTFChars(jStr, chars);
    return str;
}

// -----------------------------
// JNI exports
// -----------------------------

extern "C" {

// void initCore()
JNIEXPORT void JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_initCore(JNIEnv* env, jobject thiz) {
    initCore();
}

// boolean loadBiosPart(String kind, byte[] data)
JNIEXPORT jboolean JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_loadBiosPart(JNIEnv* env, jobject thiz,
                                                       jstring kind, jbyteArray data) {
    std::string k = jstringToStdString(env, kind);
    jsize len = env->GetArrayLength(data);
    jbyte* buf = env->GetByteArrayElements(data, nullptr);

    bool ok = loadBiosPart(k.c_str(), reinterpret_cast<uint8_t*>(buf), len);

    env->ReleaseByteArrayElements(data, buf, JNI_ABORT);
    return ok ? JNI_TRUE : JNI_FALSE;
}

// void step()
JNIEXPORT void JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_step(JNIEnv* env, jobject thiz) {
    step();
}

// String getDebugState()
JNIEXPORT jstring JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_getDebugState(JNIEnv* env, jobject thiz) {
    const char* state = getDebugState();
    return env->NewStringUTF(state);
}

// boolean isDebugReady()
JNIEXPORT jboolean JNICALL
Java_com_maxrblx1_sandboxsx2_NativeEmulator_isDebugReady(JNIEnv* env, jobject thiz) {
    return isDebugReady() ? JNI_TRUE : JNI_FALSE;
>>>>>>> 98c30722c5cc4b68755cfb441b2bedf3dede0946
}

} // extern "C"