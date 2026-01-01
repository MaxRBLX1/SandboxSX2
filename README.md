# SandboxSX2 🛡️🎮

**SandboxSX2** is a revolutionary, sandboxed PlayStation 2 emulator designed for Android. It prioritizes safety, universal compatibility, and performance on budget-friendly devices.

## 🚀 Vision
Most emulators try to bridge hardware directly, which leads to crashes and instability on different phones. SandboxSX2 builds a **complete, safe world in memory** (a sandbox) and runs the PS2 logic purely within that environment. This makes it compatible with almost any Android device, from high-end flagships to $50 budget phones.

## ✨ Features (v0.5)
- **Safe Sandboxing**: No root or special permissions required.
- **EE CPU Core**: A high-speed 32-bit MIPS interpreter.
- **DMA Highway**: Simulates the 10-channel data bus for ultra-fast internal transfers.
- **Software Graphics (GS)**: A custom-built rendering engine that doesn't rely on complex GPU drivers.
- **Supersonic Performance**: Optimized to run millions of instructions per second.
- **Simplified Setup**: Automatically creates a `ps2_bios` folder in user-accessible storage.

## 🛠️ Built With
- **Frontend**: Kotlin & Jetpack Compose
- **Core**: C++17 (Native Core)
- **Interface**: JNI (Java Native Interface)

## 📁 Getting Started
1. Install the app.
2. Locate the folder: `Android/data/com.maxrblx1.sandboxsx2/files/ps2_bios/`
3. Drop your PS2 BIOS (`.bin`) file there.
4. Launch the app and hit **"Init + Load BIOS"**.
5. Press **"Play"** and watch the magic happen!

## 🚧 Current Status: Milestone 7
The emulator now features full Slim V12 BIOS compatibility, refined interrupt handling, and a synchronized DMA highway for improved boot stability.

---
*Developed with a focus on Advanced Agentic Coding and High-Security Sandboxing.*
