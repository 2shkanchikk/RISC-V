#pragma once
#include <cstdint>

constexpr uint32_t kScreenWidth = 320;
constexpr uint32_t kScreenHeight = 200;
constexpr uint32_t kDisplay_Size = kScreenWidth * kScreenHeight * 4;

class Display{
    uint8_t* video{nullptr};

    public:
        Display();
        ~Display();

        uint8_t read8(uint32_t addr);
        uint16_t read16(uint32_t addr);
        uint32_t read32(uint32_t addr);

        void write8(uint32_t addr, uint8_t value);
        void write16(uint32_t addr, uint16_t value);
        void write32(uint32_t addr, uint32_t value);

        uint32_t* getVideo();

        Display(const Display&) = delete;
        Display& operator = (const Display&) = delete;
};