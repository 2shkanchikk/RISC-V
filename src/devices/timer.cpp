#include "timer.h"
#include <cstdio>

uint64_t Timer::count_now() const{
    using namespace std::chrono;
    return uint64_t(duration_cast<milliseconds>(steady_clock::now() - start).count());
}

uint8_t Timer::read8(uint32_t addr) const{
    uint64_t now = count_now();
    if (addr == 0x10){
        return now >= interruption_time ? 1 : 0;
    }

    return 0;
}

uint32_t Timer::read32(uint32_t addr) const{
    uint64_t now = count_now();
    switch (addr){
        case 0x00: {return uint32_t(now);}
        case 0x04: {return uint32_t(now >> 32);}
        case 0x08: {return uint32_t(interruption_time);}
        case 0x0C: {return uint32_t(interruption_time >> 32);}
        default: {return 0;}
    }
}

void Timer::write32(uint32_t addr, uint32_t value){
    switch(addr){
        case 0x08: {
            interruption_time = (interruption_time & 0xFFFFFFFF00000000) | uint64_t(value);
            break;
        }

        case 0x0C: {
            interruption_time = (interruption_time & 0x00000000FFFFFFFF) | (uint64_t(value) << 32);
            break;
        }

        default: {
            break;
        }
    }
}

void Timer::update(){
    if (irq_active) {return;}

    if (count_now() >= interruption_time){
        irq_active = true;
        printf("set irq active\n"); //test
        if(setIRQ) {setIRQ(true);}
    }
}