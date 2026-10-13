#include "display.h"

Display::Display(){
    video = new uint8_t[kDisplay_Size]();
}

Display::~Display(){
    delete[] video;
}

uint8_t Display::read8(uint32_t addr){
    if (addr >= kDisplay_Size) return 0;
    return video[addr];
}

uint16_t Display::read16(uint32_t addr){
    if (addr + 1 >= kDisplay_Size) return 0;
    return (uint16_t)video[addr] | (uint16_t)video[addr + 1] << 8;
}

uint32_t Display::read32(uint32_t addr){
    if (addr + 3 >= kDisplay_Size) return 0;
    return (uint32_t)video[addr] | (uint32_t)video[addr + 1] << 8 | (uint32_t)video[addr + 2] << 16 | (uint32_t)video[addr + 3] << 24;
}

void Display::write8(uint32_t addr, uint8_t value){
    if (addr >= kDisplay_Size) return;
    video[addr] = value;
}

void Display::write16(uint32_t addr, uint16_t value){
    if (addr + 1 >= kDisplay_Size) return;
    video[addr]     = (uint8_t)(value & 0xFF);
    video[addr + 1] = (uint8_t)(value >> 8);
}

void Display::write32(uint32_t addr, uint32_t value){
    if (addr + 3 >= kDisplay_Size) return;
    video[addr]     = (uint8_t)(value & 0xFF);
    video[addr + 1] = (uint8_t)((value >> 8) & 0xFF);
    video[addr + 2] = (uint8_t)((value >> 16) & 0xFF);
    video[addr + 3] = (uint8_t)(value >> 24);
}

uint32_t* Display::getVideo(){
    return reinterpret_cast<uint32_t*>(video);
}