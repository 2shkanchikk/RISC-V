#include "core/bus.h"
#include "core/cpu.h"
#include "loader/loader.h"
#include "platform/platform.h"
#include <cstdio>

int main(int argc, char** argv){
    Bus bus;
    CPU cpu(&bus);
    uint64_t steps = 0;
    const uint64_t kMaxSteps = 10000000000ULL;

    if (!loadBinary(bus, argv[1], 0x80000000)) return 1;

    while (!cpu.isStopped()) {
        cpu.cycle();
        ++steps;
    }

    cpu.dump();

    save_ppm("frame.ppm", bus.getDisplay().getVideo(), kScreenWidth, kScreenHeight);

    if (!cpu.isStopped()) {
    printf("TIMEOUT: no halt after %llu steps\n", (unsigned long long)steps);
    return 2;
    }

    if (cpu.getReg(10) != 0){
        printf("FAIL, test %u\n", cpu.getReg(3));
        return 1;
    }
    
    return 0;
}