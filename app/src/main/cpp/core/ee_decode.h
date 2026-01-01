#pragma once
#include <cstdint>

struct DecodedOp {
    [[maybe_unused]] uint32_t raw;
    uint8_t  op;   // primary opcode
    uint8_t  rs, rt, rd;
    uint8_t  sa;
    uint8_t  func; // Renamed from funct to avoid typo warning
    int16_t  imm;
    uint32_t target; // J-type
};

DecodedOp decode(uint32_t raw);