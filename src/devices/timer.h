#pragma once
#include <chrono>
#include <cstdint>
#include <functional>

constexpr uint32_t kTimer_Size = 0x11;

class Timer{
    std::chrono::steady_clock::time_point start{std::chrono::steady_clock::now()};
    uint64_t interruption_time = UINT64_MAX;
    bool irq_active {false};
    uint64_t count_now() const;

    public:
        std::function<void(bool)> setIRQ;
        uint8_t read8(uint32_t addr) const;
        uint32_t read32(uint32_t addr) const;

        void write32(uint32_t addr, uint32_t value);

        void update();
};