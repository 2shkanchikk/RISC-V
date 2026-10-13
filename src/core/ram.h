#pragma once
#include <cstdint>

constexpr uint32_t kRAM_Size = 32 * 1024 * 1024;

class RAM{
    uint8_t* data{nullptr};

    public:
        RAM();
        ~RAM();

        uint8_t read8(uint32_t addr) const;
        uint16_t read16(uint32_t addr) const;
        uint32_t read32(uint32_t addr) const;

        void write8(uint32_t addr, uint8_t value);
        void write16(uint32_t addr, uint16_t value);
        void write32(uint32_t addr, uint32_t value);

        RAM(const RAM&) = delete;
        RAM& operator = (const RAM&) = delete;
};