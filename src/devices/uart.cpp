#include "uart.h"
#include <cstdint>
#include <cstdio>

uint8_t UART::read8(uint32_t addr){
    return 0;
}

void UART::write8(uint32_t addr, uint8_t value){
    if (addr == 0) {
        putchar(value);
        fflush(stdout);
    }
}