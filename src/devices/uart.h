#pragma once
#include <cstdint>

class UART{
    public:
        uint8_t read8(uint32_t addr);
        void write8(uint32_t addr, uint8_t value);
};