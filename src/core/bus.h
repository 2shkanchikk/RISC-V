#pragma once
#include "ram.h"
#include "../devices/uart.h"
#include "../devices/display.h"
#include "../devices/timer.h"
#include "../devices/keyboard.h"
#include <cstdint>

class Bus{
    static constexpr uint32_t kRAM_Base = 0x80000000;
    static constexpr uint32_t kUART_Base = 0x10000000;
    static constexpr uint32_t kUART_Size = 0x100;
    static constexpr uint32_t kDisplay_Base = 0x20000000;
    static constexpr uint32_t kTimer_Base = 0x30000000;
    static constexpr uint32_t kKeyboard_Base = 0x40000000;
    static constexpr uint32_t kKeyboard_Size = 0x10000000;
    static constexpr uint32_t kTimer_Size = 0x4;
    RAM ram;
    UART uart;
    Display display;
    Timer timer;
    Keyboard keyboard;

    public:
        uint8_t read8(uint32_t addr);
        uint16_t read16(uint32_t addr);
        uint32_t read32(uint32_t addr);

        void write8(uint32_t addr, uint8_t value);
        void write16(uint32_t addr, uint16_t value);
        void write32(uint32_t addr, uint32_t value);

        Display& getDisplay();
        Keyboard& getKeyboard();
};