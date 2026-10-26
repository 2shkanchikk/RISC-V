#pragma once
#include <cstdint>
#include <string>
#include <cstdio>

class Bus;

class HostFS{
    uint32_t arg0{}, arg1{}, arg2{};
    uint32_t result {};
    Bus* bus;
    static constexpr uint8_t kMaxFiles = 16;
    FILE* files[kMaxFiles]{nullptr};
    std::string path{"files/"};
    int find_free_slot();

    public:
        HostFS(Bus*);
        ~HostFS();
        void write32(uint32_t addr, uint32_t value);
        uint32_t read32(uint32_t addr);
        void execute(uint32_t value);

        HostFS(const HostFS&) = delete;
        HostFS& operator = (const HostFS&) = delete;
};