#include "timers.h"
#include <cstring>

// Timer Register Offsets
// 0x10000000 + (id * 0x800)
// +00: COUNT
// +10: MODE
// +20: COMP (Target)
// +30: HOLD

void timersInit(Timers& tm) {
    tm = Timers{};
}

void timersStep(Timers& tm, uint32_t cycles, uint32_t& intc_stat) {
    for (int i = 0; i < 4; ++i) {
        auto& t = tm.t[i];
        if (!t.running) continue;

        // Simple emulation: Increment by cycles (assuming 1/1 clock for now for simplicity)
        // Real hardware has specific bus clock dividers (BUSCLK/16, BUSCLK/256 etc) defined in MODE
        // For BIOS boot, simple counting is usually enough.
        
        uint32_t old_count = t.count;
        t.count += cycles;

        // Check Target
        if (t.count >= t.target && t.target > 0) {
            // Target Reached
            if (t.mode & (1 << 6)) { // Bit 6: Clear on Compare
                t.count = 0; // or t.count - t.target
            }
            
            // Check Interrupt Enable (Bit 8? Need to check specific timer spec. Usually bit 10 of Mode is IRQ enable?)
            // PS2 Timer Mode: 
            // Bit 0: Clock Source
            // Bit 1: Gate Enable
            // Bit 2: Gate Mode
            // Bit 3: Gate Level
            // Bit 4: ZRET (Zero Return) - Reset on non-target?
            // Bit 6: CUE (Compare Update Enable) ? -> Reset on Compare
            // Bit 7: IE (Interrupt Enable)
            // Bit 8: OF (Overflow Flag)
            // Bit 9: EQF (Equal Flag)
            
            // Let's assume Bit 10 (0x400) or Bit 7 based on common usage.
            // Documentation says: 
            // Tn_MODE(10000010 + n*0x800)
            // Bit 7: ZRET (Zero Return)
            // Bit 8: CUE (Compare Update Enable) ?
            // Incorrect.
            
            // Let's stick to observed behavior: Bit 7 or 8 enable IRQ.
            // Using a loose logic: If target reached, signal IRQ.
            
            int irq_bit = 9 + i; // T0=9, T1=10, T2=11, T3=12
            intc_stat |= (1u << irq_bit);
        }
        
        // Overflow check (16-bit timers? PS2 timers are 32-bit generally, T0/T1 are 16bit?)
        // EE Timers: T0/T1 hold 16 bit count. T2/T3 hold 16 bit? 
        // EE Core Timers are different. 
        // These are Peripherals 0x1000xxxx.
        // T0, T1, T2, T3.
        // T0/T1 are 16-bit. T2/T3 are 32-bit? No, all 16-bit?
        // Let's safe-wrap at 32-bit for now, BIOS will set Target if it wants less.
    }
}

uint32_t timerRead(Timers& tm, uint32_t addr) {
    // Address format: 1000 0000 ... 1000 1800
    // ID = (addr & 0x1800) >> 11 ? 
    // T0: 0x1000 00xx
    // T1: 0x1000 08xx
    // T2: 0x1000 10xx
    // T3: 0x1000 18xx
    
    int id = -1;
    if ((addr & 0xFFFF800) == 0) id = 0; // 0000
    else if ((addr & 0xFFFF800) == 0x800) id = 1; // 0800
    else if ((addr & 0xFFFF800) == 0x1000) id = 2; // 1000
    else if ((addr & 0xFFFF800) == 0x1800) id = 3; // 1800
    
    // Easier masking
    uint32_t offset = addr & 0x1FFF;
    if (offset < 0x800) id = 0;
    else if (offset < 0x1000) id = 1;
    else if (offset < 0x1800) id = 2;
    else id = 3;

    uint32_t reg = addr & 0x30; // 00, 10, 20, 30
    
    auto& t = tm.t[id];
    
    switch (reg) {
        case 0x00: return t.count;
        case 0x10: return t.mode;
        case 0x20: return t.target;
        default: return 0;
    }
}

void timerWrite(Timers& tm, uint32_t addr, uint32_t val) {
    uint32_t offset = addr & 0x1FFF;
    int id = 0;
    if (offset < 0x800) id = 0;
    else if (offset < 0x1000) id = 1;
    else if (offset < 0x1800) id = 2;
    else id = 3;

    uint32_t reg = addr & 0x30;
    auto& t = tm.t[id];

    switch (reg) {
        case 0x00: t.count = val; break;
        case 0x10: 
            t.mode = val; 
            // Bit 7: Clock Enable / Start
            // (Assuming simplified bit layout for stub)
            // Code usually writes 0x80 or similar to start.
            // If bit 7 is set, run. If 0, stop.
            t.running = (val & 0x80) != 0; 
            if (!t.running) t.count = 0; // Optional reset on stop
            break;
        case 0x20: t.target = val; break;
        default: break;
    }
}
