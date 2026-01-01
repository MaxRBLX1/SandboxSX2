#include "mem_map.h"
#include <cstring>
#include <cstdio>

bool memInit(Mem& m) {
    m.ram.resize(2 * 1024 * 1024);
    std::memset(m.ram.data(), 0, m.ram.size());

    m.tick = 0;
    m.intc_stat = 0;
    m.romMap.clear();

    m.rom0.clear(); m.rom0Size = 0;
    m.rom1.clear(); m.rom1Size = 0;
    m.rom2.clear(); m.rom2Size = 0;
    m.erom.clear(); m.eromSize = 0;
    m.nvm.clear();  m.nvmSize  = 0;

    return true;
}

uint32_t memRead32(const Mem& m, uint32_t addr) {
    if (addr + 4 <= m.ram.size()) {
        return *reinterpret_cast<const uint32_t*>(&m.ram[addr]);
    }

    uint32_t base = addr & 0xFFF00000;
    auto it = m.romMap.find(base);
    if (it != m.romMap.end()) {
        const MemRegion& r = it->second;
        uint32_t offset = addr - r.base;
        if (offset + 4 <= r.size) {
            return *reinterpret_cast<const uint32_t*>(r.data + offset);
        }
    }
    return 0;
}

void memWrite32(Mem& m, uint32_t addr, uint32_t value) {
    if (addr + 4 <= m.ram.size()) {
        *reinterpret_cast<uint32_t*>(&m.ram[addr]) = value;
    }
}

void memWriteBlock(Mem& m, uint32_t addr, const uint8_t* src, size_t size) {
    if (!src || size == 0) return;
    if (addr + size <= m.ram.size()) {
        std::memcpy(&m.ram[addr], src, size);
    }
}

void memMapRom(Mem& m, uint32_t physAddr, const uint8_t* data, size_t size) {
    if (!data || size == 0) return;
    m.romMap[physAddr] = MemRegion{physAddr, static_cast<uint32_t>(size), data};
}

void memMapAliasKseg1(Mem& m, uint32_t aliasAddr, uint32_t physAddr, size_t size) {
    auto it = m.romMap.find(physAddr);
    if (it != m.romMap.end() && it->second.data) {
        m.romMap[aliasAddr] = MemRegion{aliasAddr, static_cast<uint32_t>(size), it->second.data};
    }
}

void ps2CoreLoadNVM(const uint8_t* data, int length) {
    if (!data || length <= 0) return;
    if (length >= 6) {
        char mac[18];
        std::snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
                      data[0], data[1], data[2], data[3], data[4], data[5]);
        std::printf("Loaded NVM MAC: %s\n", mac);
    }
}