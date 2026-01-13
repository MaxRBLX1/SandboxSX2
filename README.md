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

## ✨ SandboxSX2 v0.5 Features

- **Full VM Execution Loop**
  - Implements the fetch → decode → execute → synchronize → repeat pipeline
  - BIOS instructions now run continuously, advancing PC and cycles in real time

- **BIOS Execution**
  - Loads and executes Slim V12 BIOS successfully
  - Program Counter (PC) progression visible in debug overlay

- **Debug Overlay**
  - Live reporting of EE state: ticks, cycles, and PC values
  - Provides visibility into VM loop health and instruction flow

- **GS + DMA Stubs**
  - `Native_setGsReg` and `gsProcessGifPacket` implemented
  - DMA highway simulated with safe linkage for future expansion

- **Multi‑ABI Support**
  - Clean builds for `arm64-v8a`, `armeabi-v7a`, `x86`, and `x86_64`
  - Ensures compatibility across budget and flagship devices

- **Kotlin Coroutine Driver**
  - External tick loop control with pause/resume capability
  - UI button (“Pause VM Loop”) wired to coroutine driver

- **Safe Sandboxing**
  - Runs entirely in user space, no root or system modifications
  - Predictable resource usage, thermal‑friendly design

----

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


## 🎯 The Big Picture  
SandboxSX2 is built step by step:  
- Interpreter only → BIOS executes, but slow  
- JIT + GPU GS renderer → Games boot and run at playable speed, even on low‑end devices  
- Optimized audio + DMA sync → Smooth experience without stutter

---

## 🛣 Roadmap

- v0.1 → EE CPU skeleton ✅  
- v0.2 → Instruction decoder + BIOS loader ✅  
- v0.3 → Memory map (RAM, scratchpad, IOP) ✅  
- v0.4 → GS / IOP / SPU2 stubs ✅  
- v0.5 → Full VM execution loop (first real emulation milestone) ✅ 
- v0.6 → Instruction Semantics
- v0.7 → Interrupts, Timers & DMA Refinement
- v0.8 → Hybrid JIT + Graphics Synthesizer (GS) Rendering
- v0.9 → SPU2 Audio + IOP Expansion
- v1.0 → Public Beta Release (Playable)

---

## 👤 About the Developer

SandboxSX2 is developed entirely by **MaxRBLX1**, a solo developer focused on **ethical emulation**, **low-end accessibility**, and **transparent engineering**.

---

## 📜 License

MIT License — free to use, modify, and distribute.

---

## 📥 Installation

1. https://github.com/MaxRBLX1/SandboxSX2/releases/tag/0.5  
2. Place BIOS files in:  
   ```
   /Android/data/com.maxrblx1.sandboxsx2/bios/
   ```
3. Launch SandboxSX2 and press **Init Core**
4. Watch the full VM loop in Action

---

## 🌌 Community

Join the SandboxSX2 community on Discord:  
https://discord.gg/qv7cwecm

