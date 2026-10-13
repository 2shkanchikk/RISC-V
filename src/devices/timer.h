#pragma once
#include <chrono>
#include <cstdint>

class Timer{
    std::chrono::steady_clock::time_point start{std::chrono::steady_clock::now()};
    public:
        uint32_t read32(uint32_t addr) const;
};