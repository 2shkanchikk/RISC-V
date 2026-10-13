#include "timer.h"

uint32_t Timer::read32(uint32_t addr) const{
    using namespace std::chrono;
    return (uint32_t(duration_cast<milliseconds>(steady_clock::now() - start).count()));
}