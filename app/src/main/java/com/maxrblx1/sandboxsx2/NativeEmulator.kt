package com.maxrblx1.sandboxsx2

object NativeEmulator {
    init {
        System.loadLibrary("sandboxsx2") // Matches your CMake library name
    }

    external fun initCore()
    external fun loadBiosPart(kind: String, data: ByteArray): Boolean
    external fun step()
    external fun getDebugState(): String
    external fun isDebugReady(): Boolean
}