package com.maxrblx1.sandboxsx2

class NativeEmulator {

    external fun nativeTick()
    external fun nativeGetPC(): Int
    external fun nativeGetTickCount(): Long
    external fun nativeGetDebugState(): String

    // BIOS loader — accepts part name and byte array
    external fun nativeLoadBiosPart(part: String, bytes: ByteArray): Boolean

    companion object {
        init {
            System.loadLibrary("ps2native") // match your CMake target name
        }

        val instance = NativeEmulator()
    }
}