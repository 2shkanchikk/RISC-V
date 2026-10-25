#include "core/bus.h"
#include "core/cpu.h"
#include "loader/loader.h"
#include "platform/platform.h"
#include <cstdio>

int main(int argc, char** argv){
    Bus bus;
    CPU cpu(&bus);
    Platform platform("RISC-V", 960, 600, kScreenWidth, kScreenHeight);
    uint64_t steps = 0;
    const uint64_t kMaxSteps = 10000000000ULL;

    if (!loadBinary(bus, argv[1], 0x80000000)) return 1;

    save_ppm("frame.ppm", bus.getDisplay().getVideo(), kScreenWidth, kScreenHeight);

    bool running = true;

    while (!cpu.isStopped() && running) {
    for (int i = 0; i < 100000 && !cpu.isStopped(); ++i) {
        cpu.cycle();
    }

    running = platform.processInput(bus.getKeyboard());
    platform.update(bus.getDisplay().getVideo());
    }

    while (running){
        running = platform.processInput(bus.getKeyboard());
        platform.update(bus.getDisplay().getVideo());
    }

    cpu.dump();
    
    return 0;
}