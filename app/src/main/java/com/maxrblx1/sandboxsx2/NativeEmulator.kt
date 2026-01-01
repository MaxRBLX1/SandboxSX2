package com.maxrblx1.sandboxsx2

<<<<<<< HEAD
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
=======
object NativeEmulator {
    init {
        System.loadLibrary("sandboxsx2") // Matches your CMake library name
    }

    external fun initCore()
    external fun loadBiosPart(kind: String, data: ByteArray): Boolean
    external fun step()
    external fun getDebugState(): String
    external fun isDebugReady(): Boolean
>>>>>>> 98c30722c5cc4b68755cfb441b2bedf3dede0946
}