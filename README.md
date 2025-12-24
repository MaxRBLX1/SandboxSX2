
<img width="512" height="512" alt="Untitled" src="https://github.com/user-attachments/assets/25b1f306-4b86-41c4-af1c-63e9934458f2" />





# SandboxSX2

SandboxSX2 is a modular, privacy‑first PlayStation 2 emulator for Android.  
It is built from the ground up by **MaxRBLX1**, a solo developer with a vision of universal access, safety, and community‑driven customization.  

---

## 🌍 Vision: Universal Access
Most emulators focus on Snapdragon flagships, leaving millions of players excluded.  
SandboxSX2 takes a different path:

- **Universal Processor Support** → Works on any ARM or x86 CPU, not limited to Snapdragon.  
- **Inclusive Design** → Runs on Android 6.0+ (including Android Go edition).  
- **Safety & Stability** → Optimized for smooth performance without risking device health.  
- **Community Empowerment** → Modders, AI coders, and everyday players can contribute and customize freely.  

SandboxSX2 is more than an emulator — it’s a solo‑built platform for nostalgia, creativity, and inclusion.

---

## ✨ Features (v0.2)
- Real instruction decoder (halts safely on unknown instructions).  
- BIOS loader with auto‑created folders for ROM, ROM1, ROM2, EROM, NVM, MEC.  
- JNI bridge between Kotlin and native C++.  
- Debug panel showing PC, cycles, decoded instruction, registers, and BIOS sizes.  
- Scrollable UI with buttons for Init Core, Refresh Debug Info, Step CPU, Check Debug Ready, Rescan BIOS.  
- BIOS status toggle with ✅/❌ indicators.  
- Friendly warning message explaining required vs optional BIOS parts.  

---

## 📂 Project Structure
- `app/` → Android front‑end (Jetpack Compose UI).  
- `jni/` → Native C++ core.  
- `bios/` → User BIOS files (ROM, ROM1, ROM2, EROM, NVM, MEC).  
- `README.md` → Project overview.  
- `index.html` → SandboxSX2 landing page.  

---

## ⚠️ BIOS Requirements
- **ROM** → Required for the emulator to run.  
- **ROM1, ROM2, EROM, NVM, MEC** → Optional. Missing files may reduce compatibility but won’t stop the emulator.  

---

## 📱 System Requirements
- **Android** → 6.0+ (including Android Go edition).  
- **Processor** → Universal support — any ARM or x86 CPU.  
- **RAM** → 2 GB minimum, up to 16 GB supported.  

---

## 🚀 Roadmap
- v0.1 → EE CPU skeleton ✅  
- v0.2 → Instruction decoder + BIOS loader ✅  
- v0.3 → Memory map (RAM, scratchpad, IOP).  
- v0.4 → GS/IOP/SPU2 stubs.  
- v0.5 → Full VM loop.  

---

## 👤 About the Developer
SandboxSX2 is built entirely by **MaxRBLX1**, a solo developer who believes in:  
- Privacy‑first design.  
- Universal device support.  
- Stability and safety for all users.  
- Creative touches that make the emulator emotionally resonant.  

This project is a labor of passion — proving that one developer can build something powerful, inclusive, and community‑driven.

---

## 🤝 Community
SandboxSX2 is open to contributions.  
Users, modders, and AI coders are welcome to help shape its future.  

---

## 📜 License
MIT License — free to use, modify, and share.
