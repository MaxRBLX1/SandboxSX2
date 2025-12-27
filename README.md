
<img width="512" height="512" alt="Untitled" src="https://github.com/user-attachments/assets/25b1f306-4b86-41c4-af1c-63e9934458f2" />





## SandboxSX2
SandboxSX2 is a modular, privacy‑first PlayStation 2 emulator for Android. Built from the ground up by MaxRBLX1, it focuses on universal access, device safety, and transparent development — rather than chasing flagship‑only performance.

## 🌍 Vision: Universal Access
Most emulators prioritize Snapdragon flagships, leaving millions of players excluded. SandboxSX2 takes a different path — one built on accessibility, safety, and respect for device limits.
- Universal Processor Support → Works on any ARM or x86 CPU, without chipset lock‑ins or vendor bias.
- Inclusive Design → Runs on Android 6.0+, including Android Go Edition, so older and low‑end devices are not abandoned.
- Safety & Stability by Design → Uses a sandboxed execution model with controlled CPU and memory usage, avoiding unsafe native execution and aggressive optimization techniques that can overheat or damage devices.
SandboxSX2 is more than an emulator — it’s a solo‑built platform for nostalgia, creativity, and inclusion, designed to run responsibly on every supported device.

## 🔒 Why SandboxSX2 Is Safe
SandboxSX2 treats safety as a core architectural goal, not an optional feature.
- Sandboxed Core Execution → Emulation logic runs inside a controlled sandbox, limiting system access and preventing uncontrolled native code execution.
- Predictable Resource Usage → No unsafe JIT tricks, no forced overclocks, and no background services. CPU and memory usage remain bounded and transparent, even on low‑power devices.
- Thermal & Battery Friendly → Prioritizes stable operation over peak benchmark numbers, reducing overheating, throttling, and long‑term battery stress.
- No System Modification →
- No root required
- No kernel, driver, or system file changes
- Runs entirely in user space like a normal Android app
- Privacy‑First & Transparent →
- No ads
- No trackers
- No data collection
- No hidden network activity
SandboxSX2 will not brick devices, spy on users, or silently push hardware beyond safe limits.

## 🚀 SandboxSX2 v0.4 Features
- GS/IOP/SPU2 stubs integrated
- Each subsystem now has a placeholder with safe linkage.
- Debug panel reports their status (IOP=OK, GS=STUB, SPU2=STUB).
- BIOS boot flow stabilized
- BIOS regions (ROM, ROM1, ROM2, EROM, NVM) mapped cleanly.
- Safe boot sequence with “Ready” status flag.
- Instruction stepping expanded
- Cycle counter and PC reporting improved.
- Decoding shows opcode + human‑readable form (e.g., SLL $0, $0, 0).

## 📂 Project Structure
- app/ → Android front‑end (Jetpack Compose UI)
- jni/ → Native C++ core
- bios/ → User BIOS files (ROM, ROM1, ROM2, EROM, NVM, MEC)
- README.md → Project overview
- index.html → SandboxSX2 landing page

## ⚠️ BIOS Requirements
- ROM → Required for the emulator to run
- ROM1, ROM2, EROM, NVM, MEC → Optional. Missing files may reduce compatibility but will not prevent execution

## 📱 System Requirements
- Android → 6.0+ (including Android Go Edition)
- Processor → Universal support — any ARM or x86 CPU
- RAM → 2 GB minimum, up to 16 GB supported

## 🚀 Roadmap
- v0.1 → EE CPU skeleton ✅
- v0.2 → Instruction decoder + BIOS loader ✅
- v0.3 → Memory map (RAM, scratchpad, IOP) ✅
- v0.4 → GS / IOP / SPU2 stubs ✅
- v0.5 → Full VM loop

## 👤 About the Developer
SandboxSX2 is built entirely by MaxRBLX1 — a solo developer focused on ethical emulation, low‑end accessibility, and transparent engineering.

## 📜 License
MIT License — free to use, modify, and share.

## 📥 Installation
- Download the latest APK from https://github.com/MaxRBLX1/SandboxSX2/releases/tag/0.4
- Place your BIOS files in /Android/data/com.example.sandboxsx2/bios/
- Launch SandboxSX2 and press Init Core
  
## 🌌 Join the Community
Join SandboxSX2 Community on Discord: https://discord.gg/Purda4EX
