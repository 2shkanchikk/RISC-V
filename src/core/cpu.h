#pragma once
#include "bus.h"
#include <cstdint>
#include <cstdio>

class CPU{
    uint32_t x[32]{0};
    uint32_t PC{0x80000000};
    Bus* bus;
    bool stopped{false};
    uint32_t CSR[4096]{0};
    static constexpr uint32_t MSTATUS = 0x300;
    static constexpr uint32_t MIE = 0x304;
    static constexpr uint32_t MTVEC = 0x305;
    static constexpr uint32_t MSCRATCH = 0x340;
    static constexpr uint32_t MEPC = 0x341;
    static constexpr uint32_t MCAUSE = 0x342;
    static constexpr uint32_t MTVAL = 0x343;
    static constexpr uint32_t MIP = 0x344;

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

    void trap(uint32_t cause, uint32_t tval);

    public:
        CPU(Bus* bus);
        void cycle();
        void setIRQ(bool enabled);
        bool isStopped() const { return stopped; }
        uint32_t getPC() const { return PC; }
        bool hasCSR(uint32_t addr) const;
        void dump() const;
        uint32_t getReg(int number);
};
