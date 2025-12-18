# SandboxSX2
A modular, privacy-first PS2 emulator for Android.  
Built from scratch with a sandboxed BIOS core, safe memory boundaries, and a modern Kotlin + C++ architecture.

## Features (Current)
- BIOS sandbox core (safe, bounded, deterministic)
- Native C++ core with JNI bridge
- Kotlin/Compose debug UI
- Real BIOS loading and instruction fetch
- PC stepping and cycle tracking
- Works on ARMv7, ARM64, x86, x86_64

## Requirements (Full Version)
- Android 8.0+
- **Not supported:** Android Go Edition
- 2GB RAM minimum
- ARMv7/ARMv8 CPU with NEON
- 1GB free storage

## Roadmap
- EE CPU skeleton
- Instruction decoder
- Memory map (RAM, scratchpad, IOP)
- GS/IOP/SPU2 stubs
- Full VM loop
- Lite version for Android Go (future)

## Developer
Built by **MaxRBLX1** — solo developer, creator of SandboxSX2.
