SandboxSX2 Roadmap
SandboxSX2 is a safe, universal PlayStation 2 emulator for Android — designed to run across devices of all specs, from high‑end to ultra‑low‑end. This roadmap outlines the major milestones in development.
✅ Completed Milestones
v0.1 → EE CPU Skeleton
•
Established emulator project structure
•
Stubbed EE CPU core
•
Defined modular architecture for future expansion
v0.2 → Instruction Decoder + BIOS Loader
•
Implemented instruction decoder framework
•
Added BIOS loader support
•
Verified initial boot sequence parsing
v0.3 → Memory Map (RAM, Scratchpad, IOP)
•
Defined RAM and scratchpad regions
•
Integrated IOP memory mapping
•
Validated memory access stubs
v0.4 → GS / IOP / SPU2 Stubs
•
Stubbed Graphics Synthesizer (GS)
•
Stubbed IOP and SPU2 subsystems
•
Prepared hooks for future audio/video integration
v0.5 → Full VM Loop
•
Achieved stable Full VM Loop
•
BIOS boot with live debug overlay
•
Demonstrated stability even on ultra‑low‑end specs
v0.6 → Instruction Semantics
•
Core Arithmetic & Logic: Implemented ADD, SUB, MULT, DIV, Logical ops, and Shifts (SLL, SRL, SRA).
•
Control Flow: Implemented Branch and Jump instructions with delay slot handling.
•
Memory Access: Added Load/Store for word, halfword, and byte, plus Unaligned (LWL/LWR/SWL/SWR) and Scratchpad mapping.
•
Special Registers: HI/LO handling, PC updates, and COP0 Exception stubs.
•
Exceptions & Traps: Implemented basic exception framework with jumps to vector 0x80000000.
v0.7 → Interrupts, Timers & DMA Refinement (100% DONE)
•
Interrupt Framework: Implemented software INTC handling.
•
Hardware Timers: Refined for accurate scheduling with clock dividers.
•
DMA Refinement: Implemented Source Chain mode and DMAtag (CNT, NEXT, END) support.
•
Core Sync: Implemented 8:1 clock ratio synchronization between EE and IOP.
🚧 Current Milestone
v0.8 → GS Rendering Prototype + Hybrid JIT
•
Begin GS rendering pipeline (Vulkan/GLES)
•
Implement basic frame buffer output
•
Hybrid JIT logic for "hot" instruction blocks
•
Debug overlay for GPU events
🌌 Future Milestones
v0.9 → SPU2 Audio + IOP Expansion
•
Integrate SPU2 audio playback
•
Expand IOP subsystem functionality
•
Synchronize audio with VM loop
v1.0 → Public Beta Release (Playable)
•
Playable PS2 titles on supported Android devices
•
Full sandbox safety layer
•
Community‑driven features and accessibility
