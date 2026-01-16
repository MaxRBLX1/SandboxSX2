<p align="center">
  <img width="256" height="256" alt="SandboxSX2 Logo" src="https://github.com/user-attachments/assets/25b1f306-4b86-41c4-af1c-63e9934458f2" />
</p>

# 🎮 SandboxSX2

**SandboxSX2** is a safe, universal **PlayStation 2 emulator for Android**, developed solo by **MaxRBLX1**.  
It follows a **"Universal First"** philosophy, optimized to run on **all devices**—from flagship phones to ultra-low-end hardware like the Realme C11.  

Using a **100% sandboxed core**, SandboxSX2 delivers **stable emulation without compromising device safety, privacy, or thermal performance**.

⚠️ **Note:** SandboxSX2 is currently a **BIOS loader and debugging platform**. Full PS2 game support will come in **v1.0 (Public Beta)**.

---

## 🚀 What’s New in v0.7

v0.7 is a **major milestone**, introducing the “Nervous System” of the emulator:

- **Interrupt Framework (INTC):** Full software-level handling for CPU interrupts  
- **Hardware Timers:** Accurate scheduling using clock dividers for realistic VM speed  
- **DMA Refinement:** Source Chain mode and full DMAtag (`CNT`, `NEXT`, `END`) support  
- **Core Synchronization:** Precise 8:1 clock ratio between EE and IOP processors  
- **Live Debug Overlay:** See PC, cycles, and GIF activity in real time  

> v0.7 is **fully functional for BIOS boot and debugging**, but **games do not run yet**.

---

## 🌍 Mission: Emulation for Everyone

SandboxSX2 is designed for **millions of users on budget devices**:

- **Universal Compatibility:** Android 6.0+ (including Android Go), ARM and x86 CPUs  
- **Safe by Design:** Runs entirely in a sandbox; no root, kernel hacks, or unsafe overclocks  
- **Efficient & Ethical:** No ads, no trackers, optimized C++ code prevents overheating  

---

## ✨ Key Features (v0.7)

- **Stable VM Loop:** Continuous BIOS instruction execution for real-time debugging  
- **Advanced Instruction Set:** Full v0.6 semantics including unaligned memory access, exceptions, and special registers  
- **BIOS Loader:** Loads Slim V12 and other standard PS2 BIOS versions  
- **Hardware Monitoring:** Debug overlay showing PC, cycles, and GIF activity  
- **Multi-ABI Support:** Native binaries for `arm64-v8a`, `armeabi-v7a`, `x86`, and `x86_64`  
- **Safe Control:** Instant pause/resume and reset functionality  

> ⚠️ Games are **not playable yet**. Only BIOS loading and VM debugging is currently functional.

---

## ⚠️ BIOS / .BIN Files Required

SandboxSX2 **requires official PlayStation 2 BIOS files** in `.bin` format. These are **not included**.

- **Required:** `scphXXXX.bin` (Main BIOS)  
- **Optional:** `rom1.bin`, `rom2.bin`, `erom.bin`, `nvm.bin`, `mec.bin`  

> **How to copy BIOS files:**  
> 1. Open **ZArchiver** or any file manager on your device.  
> 2. Go to your **Downloads** folder.  
> 3. Find the `.bin` BIOS file, then **hold/tap and copy** (depending on your file manager).  
> 4. Navigate to:  
>    ```
>    Android/Data/com.maxrblx1.sandboxsx2/files/ps2_bios
>    ```  
> 5. Paste the `.bin` file here.  
>
> ⚠️ If you don’t have a BIOS file yet, you can download a free PS2 BIOS from the internet (make sure it is `.bin`).  
> Missing optional files may reduce compatibility with some games but will **not prevent the emulator from running BIOS/debug**.

---

## 🎯 Roadmap

### ✅ Completed Milestones

- **v0.1 → EE CPU Skeleton** – Project structure, stubbed EE core, modular architecture  
- **v0.2 → Instruction Decoder + BIOS Loader** – Decoder framework, BIOS loader, verified boot parsing  
- **v0.3 → Memory Map (RAM, Scratchpad, IOP)** – Defined regions, integrated IOP mapping, memory access validation  
- **v0.4 → GS / IOP / SPU2 Stubs** – Stubbed Graphics Synthesizer, IOP, SPU2; hooks for audio/video  
- **v0.5 → Full VM Loop** – Stable VM loop, BIOS boot, debug overlay, tested on low-end hardware  
- **v0.6 → Instruction Semantics** – Arithmetic & Logic ops, Branch/Jump instructions, memory access, exceptions, HI/LO/COP0 handling  
- **v0.7 → Interrupts, Timers & DMA Refinement (100% DONE)** – INTC handling, hardware timers, DMA Source Chain + DMAtag, 8:1 EE:IOP clock sync, live debug overlay  

### 🚧 Current Milestone

- **v0.8 → GS Rendering Prototype + Hybrid JIT**  
  - Vulkan/GLES pipeline  
  - Basic frame buffer output  
  - Hybrid JIT for hot instruction blocks  
  - GPU debug overlay  

### 🌌 Future Milestones

- **v0.9 → SPU2 Audio + IOP Expansion** – Audio playback integration, IOP subsystem expansion, audio sync  
- **v1.0 → Public Beta Release (Playable)** – Playable PS2 titles on all supported Android devices, full Sandbox safety layer, community-driven features  

---

## 📥 Getting Started

1. **Download:** Get the latest APK from the [Releases Page](https://github.com/MaxRBLX1/SandboxSX2/releases)  
2. **Install:** Enable “Install from Unknown Sources” on your Android device  
3. **Setup BIOS:**  
   - Install **ZArchiver** or any file manager  
   - Copy your `.bin` BIOS files to:  
     ```
     Android/Data/com.maxrblx1.sandboxsx2/files/ps2_bios
     ```  
4. **Launch:** Tap **Setup BIOS**, then **Start Engine**  
   - Only BIOS loading and VM debugging are available now; **game support will arrive in v1.0**  

---

## 📜 License & Credits

- **Developer:** MaxRBLX1 (Solo Project)  
- **License:** MIT License — Open, safe, and free  

---

## 🌐 Community

- **Discord:** [Join the SandboxSX2 Community](https://discord.gg/jTDwZ4CR)  

---

**SandboxSX2 — BIOS loader and debugging platform today, full PS2 gameplay in v1.0.**
