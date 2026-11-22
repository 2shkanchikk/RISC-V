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
    bool stepsEnabled = false;

    if (argc < 2){return 1;}

    if (!loadBinary(bus, argv[1], 0x80000000)) {return 1;}

    //save_ppm("frame.ppm", bus.getDisplay().getVideo(), kScreenWidth, kScreenHeight);

    bool running = true;
    
    while(!cpu.isStopped() && running){
        uint64_t batch = 100000;
        if(stepsEnabled && (kMaxSteps - steps < batch)){
            batch = kMaxSteps - steps;
        }

        for (uint64_t i = 0; i < batch && !cpu.isStopped(); ++i){
            cpu.cycle();
            ++steps;

            if((i & 0xFF) == 0){
                bus.update();
            }
        }

        running = platform.processInput(bus.getKeyboard());
        platform.update(bus.getDisplay().getVideo());
    }

    cpu.dump();
    
    return 0;
}