#pragma once
#include <cstdint>
#include <deque>
#include <cstddef>

class Keyboard{
    std::deque<uint32_t> queue;
    static constexpr size_t max_queue = 32;

    public:
        void push(bool pressed, uint8_t key);
        uint32_t read32(uint32_t addr);
};