# SandboxSX2 Roadmap

SandboxSX2 is a safe, universal PlayStation 2 emulator for Android — designed to run across devices of all specs, from high‑end to ultra‑low‑end.  
This roadmap outlines the major milestones in development.

---

## ✅ Completed Milestones

### v0.1 → EE CPU Skeleton
- Established emulator project structure
- Stubbed EE CPU core
- Defined modular architecture for future expansion

### v0.2 → Instruction Decoder + BIOS Loader
- Implemented instruction decoder framework
- Added BIOS loader support
- Verified initial boot sequence parsing

### v0.3 → Memory Map (RAM, Scratchpad, IOP)
- Defined RAM and scratchpad regions
- Integrated IOP memory mapping
- Validated memory access stubs

### v0.4 → GS / IOP / SPU2 Stubs
- Stubbed Graphics Synthesizer (GS)
- Stubbed IOP and SPU2 subsystems
- Prepared hooks for future audio/video integration

### v0.5 → Full VM Loop
- Achieved stable Full VM Loop
- BIOS boot with live debug overlay
- Demonstrated stability even on ultra‑low‑end specs

---

## 🚧 Current Milestone

### v0.6 → Instruction Semantics
1. **Core Arithmetic & Logic**
   - [ ] Implement ADD, SUB, MULT, DIV (signed/unsigned)
   - [ ] Logical ops: AND, OR, XOR, NOR
   - [ ] Shift ops: SLL, SRL, SRA

2. **Control Flow**
   - [ ] Branch instructions (BEQ, BNE, BLEZ, BGTZ)
   - [ ] Jump instructions (J, JAL)
   - [ ] Delay slot handling

3. **Memory Access**
   - [ ] Load/store word (LW, SW)
   - [ ] Load/store halfword & byte (LH, LB, SH, SB)
   - [ ] Scratchpad + RAM mapping validation

4. **Special Registers**
   - [ ] HI/LO register handling
   - [ ] Program Counter (PC) updates
   - [ ] COP0 stubs for system control

5. **Exceptions & Traps**
   - [ ] Basic exception handling framework
   - [ ] Debug overlay: log exception triggers

6. **Debug Overlay Expansion**
   - [ ] Show executed instruction + operands + result
   - [ ] Highlight branch decisions
   - [ ] Memory access visualization

7. **Validation**
   - [ ] Run BIOS routines and confirm expected state changes
   - [ ] Compare execution against PCSX2 logs / MIPS docs
   - [ ] Document supported instructions in README

---

## 🌌 Future Milestones

### v0.7 → Interrupts, Timers & DMA Refinement
- Implement interrupt handling framework
- Refine hardware timers for accurate scheduling
- DMA channel refinement for stable data transfers

### v0.8 → Graphics Synthesizer (GS) Rendering Prototype with Hybrid JIT to render it
- Begin GS rendering pipeline
- Implement basic frame buffer output
- Debug overlay for GPU events

### v0.9 → SPU2 Audio + IOP Expansion
- Integrate SPU2 audio playback
- Expand IOP subsystem functionality
- Synchronize audio with VM loop

### v1.0 → Public Beta Release (Playable)
- Playable PS2 titles on supported Android devices
- Full sandbox safety layer
- Community‑driven features and accessibility

---

## 🤝 Community
- GitHub: [https://github.com/MaxRBLX1/SandboxSX2](https://github.com/MaxRBLX1/SandboxSX2)  
- Discord: [https://discord.gg/YbWFtKM4](https://discord.gg/YbWFtKM4)  
- Roblox Profile: [https://www.roblox.com/users/3331791755/profile](https://www.roblox.com/users/3331791755/profile)
