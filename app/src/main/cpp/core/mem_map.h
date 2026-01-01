#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>

struct MemRegion {
    uint32_t base;
    uint32_t size;
    const uint8_t* data;
};

struct Mem {
    std::vector<uint8_t> ram;

    std::vector<uint8_t> rom0; // .bin
    std::vector<uint8_t> rom1; // .rom1
    std::vector<uint8_t> rom2; // .rom2
    std::vector<uint8_t> erom; // .erom
    std::vector<uint8_t> nvm;  // .nvm

    size_t rom0Size = 0;
    size_t rom1Size = 0;
    size_t rom2Size = 0;
    size_t eromSize = 0;
    size_t nvmSize  = 0;

    uint64_t tick = 0;
    uint32_t intc_stat = 0;

    std::unordered_map<uint32_t, MemRegion> romMap;
};

bool     memInit(Mem& m);
uint32_t memRead32(const Mem& m, uint32_t addr);
void     memWrite32(Mem& m, uint32_t addr, uint32_t value);
void     memWriteBlock(Mem& m, uint32_t addr, const uint8_t* src, size_t size);

void memMapRom(Mem& m, uint32_t physAddr, const uint8_t* data, size_t size);
void memMapAliasKseg1(Mem& m, uint32_t aliasAddr, uint32_t physAddr, size_t size);

void ps2CoreLoadNVM(const uint8_t* data, int length);