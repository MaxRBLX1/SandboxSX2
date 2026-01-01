<<<<<<< HEAD
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
=======

<img width="512" height="512" alt="Untitled" src="https://github.com/user-attachments/assets/25b1f306-4b86-41c4-af1c-63e9934458f2" />





## SandboxSX2

**SandboxSX2** is a modular, privacy-first **PlayStation 2 emulator project for Android**. Built from the ground up by **MaxRBLX1**, it prioritizes **universal access, device safety, and transparent development** rather than chasing flagship-only performance numbers.

SandboxSX2 is designed to run responsibly across a wide range of devices, including low-end and older hardware, without vendor lock-ins or unsafe optimizations.

---

## 🌍 Vision: Universal Access

Most modern emulators focus on Snapdragon flagships, leaving millions of users behind. SandboxSX2 follows a different philosophy — one centered on **accessibility, safety, and respect for device limitations**.

- **Universal Processor Support**  
  Targets all ARM and x86 CPUs, without chipset restrictions or vendor bias.

- **Inclusive Design**  
  Supports **Android 6.0+**, including **Android Go Edition**, ensuring older and low-end devices are not abandoned.

- **Safety & Stability by Design**  
  Uses a sandboxed execution model with controlled CPU and memory usage, avoiding unsafe native execution and aggressive optimizations that may overheat or damage devices.

SandboxSX2 is not just an emulator — it is a **solo-built platform for nostalgia, creativity, and inclusion**, designed to run responsibly on every supported device.

---

## 🔒 Why SandboxSX2 Is Safe

Safety is a core architectural goal of SandboxSX2, not an afterthought.

- **Sandboxed Core Execution**  
  Emulation logic runs inside a controlled sandbox, limiting system access and preventing uncontrolled native code execution.

- **Predictable Resource Usage**  
  No unsafe JIT tricks, no forced overclocks, and no background services. CPU and memory usage remain bounded and transparent, even on low-power devices.

- **Thermal & Battery Friendly**  
  Prioritizes stable operation over benchmark-driven performance, reducing overheating, throttling, and long-term battery stress.

- **No System Modification**  
  - No root required  
  - No kernel, driver, or system file changes  
  - Runs entirely in user space like a standard Android app

- **Privacy-First & Transparent**  
  - No ads  
  - No trackers  
  - No data collection  
  - No hidden network activity  

SandboxSX2 will not brick devices, spy on users, or silently push hardware beyond safe operating limits.

---

## 🚀 SandboxSX2 v0.4 Features

> **Note:** SandboxSX2 is currently a **core foundation and BIOS loader**, not a full emulator yet.

- GS / IOP / SPU2 stubs integrated  
- Subsystem placeholders with safe linkage  
- Debug panel reporting subsystem status  
  - IOP = OK  
  - GS = STUB  
  - SPU2 = STUB  
- BIOS boot flow stabilized  
- BIOS regions mapped cleanly: ROM, ROM1, ROM2, EROM, NVM  
- Safe boot sequence with “Ready” status flag  
- Instruction stepping expanded  
- Improved cycle counter and program counter reporting  
- Opcode decoding with human-readable output  
  - Example: `SLL $0, $0, 0`

---

## 📂 Project Structure

- `app/` → Android frontend (Jetpack Compose UI)  
- `jni/` → Native C++ core  
- `bios/` → User-provided BIOS files  
- `README.md` → Project documentation  
- `index.html` → SandboxSX2 landing page  

---

## ⚠️ BIOS Requirements

- **ROM** → Required  
- **ROM1, ROM2, EROM, NVM, MEC** → Optional  
  - Missing files may reduce compatibility but will not prevent execution

---

## 📱 System Requirements

- **Android:** 6.0+ (including Android Go Edition)  
- **CPU:** Universal support — ARM or x86  
- **RAM:** 2 GB minimum (up to 16 GB supported)

---

## 🛣 Roadmap

- v0.1 → EE CPU skeleton ✅  
- v0.2 → Instruction decoder + BIOS loader ✅  
- v0.3 → Memory map (RAM, scratchpad, IOP) ✅  
- v0.4 → GS / IOP / SPU2 stubs ✅  
- **v0.5 → Full VM execution loop (first real emulation milestone)**

---

## 👤 About the Developer

SandboxSX2 is developed entirely by **MaxRBLX1**, a solo developer focused on **ethical emulation**, **low-end accessibility**, and **transparent engineering**.

---

## 📜 License

MIT License — free to use, modify, and distribute.

---

## 📥 Installation

1. Download the latest APK from the Releases page  
2. Place BIOS files in:  
   ```
   /Android/data/com.example.sandboxsx2/bios/
   ```
3. Launch SandboxSX2 and press **Init Core**

---

## 🌌 Community

Join the SandboxSX2 community on Discord:  
https://discord.gg/Purda4EX
>>>>>>> 98c30722c5cc4b68755cfb441b2bedf3dede0946
