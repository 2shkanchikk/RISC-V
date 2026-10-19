#include "bus.h"
#include <cstdint>
#include <cstdio>

uint8_t Bus::read8(uint32_t addr){
    if ((addr >= kRAM_Base) && (addr < (kRAM_Base + kRAM_Size))){
        return ram.read8(addr - kRAM_Base);
    }

    if ((addr >= kDisplay_Base) && (addr < (kDisplay_Base + kDisplay_Size))){
        return display.read8(addr - kDisplay_Base);
    }

    if ((addr >= kUART_Base) && (addr < (kUART_Base + kUART_Size))){
        return uart.read8(addr - kUART_Base);
    }

    printf("Bus: read8 from unknown address %08X\n", addr);
    return 0;
}

uint16_t Bus::read16(uint32_t addr){
    if ((addr >= kRAM_Base) && (addr < (kRAM_Base + kRAM_Size))){
        return ram.read16(addr - kRAM_Base);
    }

    if ((addr >= kDisplay_Base) && (addr < (kDisplay_Base + kDisplay_Size))){
        return display.read16(addr - kDisplay_Base);
    }

    printf("Bus: read16 from unknown address %08X\n", addr);
    return 0;
}

uint32_t Bus::read32(uint32_t addr){
    if ((addr >= kRAM_Base) && (addr < (kRAM_Base + kRAM_Size))){
        return ram.read32(addr - kRAM_Base);
    }

    if ((addr >= kDisplay_Base) && (addr < (kDisplay_Base + kDisplay_Size))){
        return display.read32(addr - kDisplay_Base);
    }

    if ((addr >= kKeyboard_Base) && (addr < (kKeyboard_Base + kKeyboard_Size))){
        return keyboard.read32(addr - kKeyboard_Base);
    }

    if ((addr >= kTimer_Base) && (addr < (kTimer_Base + kTimer_Size))){
        return timer.read32(addr - kTimer_Base);
    }

    printf("Bus: read32 from unknown address %08X\n", addr);
    return 0;
}

void Bus::write8(uint32_t addr, uint8_t value){
    if ((addr >= kRAM_Base) && (addr < (kRAM_Base + kRAM_Size))){
        ram.write8(addr - kRAM_Base, value);
        return;
    }

    if ((addr >= kDisplay_Base) && (addr < (kDisplay_Base + kDisplay_Size))){
        display.write8(addr - kDisplay_Base, value);
        return;
    }

    if ((addr >= kUART_Base) && (addr < (kUART_Base + kUART_Size))){
        uart.write8(addr - kUART_Base, value);
        return;
    }

    printf("Bus: write8 to unknown address %08X\n", addr);
}

void Bus::write16(uint32_t addr, uint16_t value){
    if ((addr >= kRAM_Base) && (addr < (kRAM_Base + kRAM_Size))){
        ram.write16(addr - kRAM_Base, value);
        return;
    }

    if ((addr >= kDisplay_Base) && (addr < (kDisplay_Base + kDisplay_Size))){
        display.write16(addr - kDisplay_Base, value);
        return;
    }

    printf("Bus: write16 to unknown address %08X\n", addr);
}

void Bus::write32(uint32_t addr, uint32_t value){
    if ((addr >= kRAM_Base) && (addr < (kRAM_Base + kRAM_Size))){
        ram.write32(addr - kRAM_Base, value);
        return;
    }

    if ((addr >= kDisplay_Base) && (addr < (kDisplay_Base + kDisplay_Size))){
        display.write32(addr - kDisplay_Base, value);
        return;
    }

    printf("Bus: write32 to unknown address %08X\n", addr);
}

Display& Bus::getDisplay(){
    return display;
}

Keyboard& Bus::getKeyboard(){
    return keyboard;
}