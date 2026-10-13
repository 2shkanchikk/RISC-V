#include "ram.h"
#include <cstdio>

RAM::RAM(){
    data = new uint8_t[kRAM_Size]();
}

RAM::~RAM(){
    delete[] data;
}

uint8_t RAM::read8(uint32_t addr) const{
    if (addr >= kRAM_Size) return 0;
    return data[addr];
}

uint16_t RAM::read16(uint32_t addr) const{
    if (addr + 1 >= kRAM_Size) {
        printf("RAM read8 out of range: %08X\n", addr);
        return 0;
    }
    return (uint16_t)data[addr] | (uint16_t)data[addr + 1] << 8;
}

uint32_t RAM::read32(uint32_t addr) const{
    if (addr + 3 >= kRAM_Size) return 0;
    return (uint32_t)data[addr] | (uint32_t)data[addr + 1] << 8 | (uint32_t)data[addr + 2] << 16 | (uint32_t)data[addr + 3] << 24;
}

void RAM::write8(uint32_t addr, uint8_t value){
    if (addr >= kRAM_Size) return;
    data[addr] = value;
}

void RAM::write16(uint32_t addr, uint16_t value){
    if (addr + 1 >= kRAM_Size) return;
    data[addr] = (uint8_t) (value & 0xFF);
    data[addr + 1] = (uint8_t) ((value >> 8) & 0xFF);
}

void RAM::write32(uint32_t addr, uint32_t value){
    if (addr + 3 >= kRAM_Size) return;
    data[addr] = (uint8_t) (value & 0xFF);
    data[addr + 1] = (uint8_t) ((value >> 8) & 0xFF);
    data[addr + 2] = (uint8_t) ((value >> 16) & 0xFF);
    data[addr + 3] = (uint8_t) (value >> 24);
}