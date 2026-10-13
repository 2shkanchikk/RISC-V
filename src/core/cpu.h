#pragma once
#include "bus.h"
#include <cstdint>
#include <cstdio>

class CPU{
    uint32_t x[32]{0};
    uint32_t PC{0x80000000};
    Bus* bus;
    bool stopped{false};
    uint32_t csr[4096]{0};

    void execOp(uint32_t instruction);
    void execMul(uint32_t instruction); // M extension
    void execOpImm(uint32_t instruction);
    void execLoad(uint32_t instruction);
    void execStore(uint32_t instruction);
    void execBranch(uint32_t instruction);
    void execJAL(uint32_t instruction);
    void execJALR(uint32_t instruction);
    void execLUI(uint32_t instruction);
    void execAUIPC(uint32_t instruction);
    void execEnv(uint32_t instruction);

    void trap(uint32_t cause);

    public:
        CPU(Bus* bus);
        void cycle();
        bool isStopped() const { return stopped; }
        uint32_t getPC() const { return PC; }
        void dump() const;
        uint32_t getReg(int number);
};
