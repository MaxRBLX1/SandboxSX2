#include "ee_decode.h"

DecodedOp decode(uint32_t raw) {
    DecodedOp d{};
    d.raw = raw;
    d.op = (raw >> 26) & 0x3F;
    d.rs = (raw >> 21) & 0x1F;
    d.rt = (raw >> 16) & 0x1F;
    d.rd = (raw >> 11) & 0x1F;
    d.sa = (raw >> 6)  & 0x1F;
    d.func = raw & 0x3F;
    d.imm = static_cast<int16_t>(raw & 0xFFFF);
    d.target = (raw & 0x03FFFFFFu) << 2;
    return d;
}