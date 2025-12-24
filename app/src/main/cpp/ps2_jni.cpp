// ps2_jni.cpp
#include <jni.h>
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
}

} // extern "C"