#include "cpu.h"

CPU::CPU(Bus* bus){
    this->bus = bus;
}

void CPU::trap(uint32_t cause, uint32_t tval){
    if (CSR[0x305] == 0) {
        printf("Unhandled trap: cause %u, tval %08X at PC %08X\n", cause, tval, PC - 4);
        stopped = true;
        return;
    }
    CSR[0x341] = PC - 4;
    CSR[0x342] = cause;
    CSR[0x343] = tval;
    PC = CSR[0x305] >> 2 << 2;
}

static uint32_t rd(uint32_t instruction){
    return (instruction >> 7 & 0x1F);
}

static uint32_t funct3(uint32_t instruction){
    return (instruction >> 12 & 0x7);
}

static uint32_t rs1(uint32_t instruction){
    return (instruction >> 15 & 0x1F);
}

static uint32_t rs2(uint32_t instruction){
    return (instruction >> 20 & 0x1F);
}

static uint32_t funct7(uint32_t instruction){
    return (instruction >> 25 & 0x7F);
}

static uint32_t immI(uint32_t instruction){
    return (uint32_t)((int32_t)instruction >> 20);
}

static uint32_t immS(uint32_t instruction){
    return ((uint32_t)((int32_t)instruction >> 25) << 5) | ((instruction >> 7) & 0x1F);
}

static uint32_t immB(uint32_t instruction){
    uint32_t imm = ((instruction >> 31) & 0x1)  << 12   // imm[12]
                 | ((instruction >> 25) & 0x3F) << 5    // imm[10:5]
                 | ((instruction >> 8)  & 0xF)  << 1    // imm[4:1]
                 | ((instruction >> 7)  & 0x1)  << 11;  // imm[11]
    if (imm & (1u << 12)) imm |= 0xFFFFE000;
    return imm;
}

static uint32_t immU(uint32_t instruction){
    return (instruction & 0xFFFFF000);
}

static uint32_t immJ(uint32_t instruction){
    uint32_t imm = ((instruction >> 31) & 0x1)   << 20  // imm[20]
                 | ((instruction >> 21) & 0x3FF) << 1   // imm[10:1]
                 | ((instruction >> 20) & 0x1)   << 11  // imm[11]
                 | ((instruction >> 12) & 0xFF)  << 12; // imm[19:12]
    if (imm & (1u << 20)) imm |= 0xFFE00000;
    return imm;
}

void CPU::execOp(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t rs2 = ::rs2(instruction);
    uint32_t funct7 = ::funct7(instruction);

    if(funct7 == 0x01){
        execMul(instruction);
        return;
    }

    switch(funct3){
        case(0x0):{
            switch(funct7){
                case(0x00):{
                    x[rd] = x[rs1] + x[rs2];
                    break;
                }

                case(0x20):{
                    x[rd] = x[rs1] - x[rs2];
                    break;
                }

                default: {
                    trap(2, instruction);
                }
            }
            break;
        }

        case(0x1):{
            if(funct7 == 0x00){
                x[rd] = x[rs1] << (x[rs2] & 0x1F);
            } else {
                trap(2, instruction);
            }
            break;
        }

        case(0x2):{
            if(funct7 == 0x00){
                x[rd] = (int32_t(x[rs1]) < int32_t(x[rs2])) ? 1 : 0;
            } else {
                trap(2, instruction);
            }
            break;
        }

        case(0x3):{
            if(funct7 == 0x00){
                x[rd] = (x[rs1] < x[rs2]) ? 1 : 0;
            } else {
                trap(2, instruction);
            }
            break;
        }

        case(0x4):{
            if(funct7 == 0x00){
                x[rd] = x[rs1] ^ x[rs2];
            } else {
                trap(2, instruction);
            }
            break;
        }

        case(0x5):{
            switch(funct7){
                case(0x00):{
                    x[rd] = x[rs1] >> (x[rs2] & 0x1F);
                    break;
                }

                case(0x20):{
                    x[rd] = (int32_t(x[rs1]) >> (x[rs2] & 0x1F));
                    break;
                }

                default: {
                    trap(2, instruction);
                }
            }
            break;
        }

        case(0x6):{
            if(funct7 == 0x00){
                x[rd] = x[rs1] | x[rs2];
            } else {
                trap(2, instruction);
            }
            break;
        }

        case(0x7):{
            if(funct7 == 0x00){
                x[rd] = x[rs1] & x[rs2];
            } else {
                trap(2, instruction);
            }
            break;
        }
    }
}

void CPU::execMul(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t rs2 = ::rs2(instruction);

    switch(funct3){
        case(0x0):{
            x[rd] = (x[rs1] * x[rs2]);
            break;
        }

        case(0x1):{
            x[rd] = uint32_t((int64_t(int32_t(x[rs1])) * int64_t(int32_t(x[rs2]))) >> 32);
            break;
        }

        case(0x2):{
            x[rd] = uint32_t((int64_t(int32_t(x[rs1])) * uint64_t(x[rs2])) >> 32);
            break;
        }

        case(0x3):{
            x[rd] = uint32_t((uint64_t(x[rs1]) * uint64_t(x[rs2])) >> 32);
            break;
        }

        case(0x4):{
            if(x[rs1] == 0x80000000 && x[rs2] == 0xFFFFFFFF){
                x[rd] = 0x80000000;
                break;
            }
            if(x[rs2] == 0){
                x[rd] = 0xFFFFFFFF;
                break;
            }
            x[rd] = uint32_t(int32_t(x[rs1]) / int32_t(x[rs2]));
            break;
        }

        case(0x5):{
            if(x[rs2] == 0){
                x[rd] = 0xFFFFFFFF;
                break;
            }
            x[rd] = x[rs1] / x[rs2];
            break;
        }

        case(0x6):{
            if(x[rs1] == 0x80000000 && x[rs2] == 0xFFFFFFFF){
                x[rd] = 0;
                break;
            }
            if(x[rs2] == 0){
                x[rd] = x[rs1];
                break;
            }
            x[rd] = uint32_t(int32_t(x[rs1]) % int32_t(x[rs2]));
            break;
        }

        case(0x7):{
            if(x[rs2] == 0){
                x[rd] = x[rs1];
                break;
            }
            x[rd] = x[rs1] % x[rs2];
            break;
        }
    }
}

void CPU::execOpImm(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t imm = immI(instruction);

    switch(funct3){
        case(0x0):{
            x[rd] = x[rs1] + imm;
            break;
        }

        case(0x1):{
            if (::funct7(instruction) == 0x00){
                x[rd] = x[rs1] << ::rs2(instruction);
            } else {
                trap(2, instruction);
            }
            break;
        }

        case(0x2):{
            x[rd] = uint32_t((int32_t(x[rs1]) < int32_t(imm)) ? 1 : 0);
            break;
        }

        case(0x3):{
            x[rd] = (x[rs1] < imm) ? 1 : 0;
            break;
        }
        
        case(0x4):{
            x[rd] = x[rs1] ^ imm;
            break;
        }

        case(0x5):{
            switch(::funct7(instruction)){
                case(0x00):{
                    x[rd] = x[rs1] >> ::rs2(instruction);
                    break;
                }

                case(0x20):{
                    x[rd] = uint32_t(int32_t(x[rs1]) >> ::rs2(instruction));
                    break;
                }

                default:{
                    trap(2, instruction);
                }
            }
            break;
        }

        case(0x6):{
            x[rd] = x[rs1] | imm;
            break;
        }

        case(0x7):{
            x[rd] = x[rs1] & imm;
            break;
        }
    }
}

void CPU::execLoad(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t imm = immI(instruction);

    switch(funct3){
        case(0x0):{
            x[rd] = uint32_t(int32_t(int8_t(bus->read8(x[rs1] + imm))));
            break;
        }

        case(0x1):{
            x[rd] = uint32_t(int32_t(int16_t(bus->read16(x[rs1] + imm))));
            break;
        }

        case(0x2):{
            x[rd] = bus->read32(x[rs1] + imm);
            break;
        }

        case(0x4):{
            x[rd] = bus->read8(x[rs1] + imm);
            break;
        }

        case(0x5):{
            x[rd] = bus->read16(x[rs1] + imm);
            break;
        }

        default:{
            trap(2, instruction);
        }
    }
}

void CPU::execStore(uint32_t instruction){
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t rs2 = ::rs2(instruction);
    uint32_t imm = immS(instruction);

    switch(funct3){
        case(0x0):{
            bus->write8(x[rs1] + imm, uint8_t(x[rs2]));
            break;
        }

        case(0x1):{
            bus->write16(x[rs1] + imm, uint16_t(x[rs2]));
            break;
        }

        case(0x2):{
            bus->write32(x[rs1] + imm, x[rs2]);
            break;
        }

        default:{
            trap(2, instruction);
        }
    }
}

void CPU::execBranch(uint32_t instruction){
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t rs2 = ::rs2(instruction);
    uint32_t imm = immB(instruction);

    switch(funct3){
        case(0x0):{
            if(x[rs1] == x[rs2]){
                PC -= 4;
                PC += imm;
            }
            break;
        }

        case(0x1):{
            if(x[rs1] != x[rs2]){
                PC -= 4;
                PC += imm;
            }
            break;
        }

        case(0x4):{
            if(int32_t(x[rs1]) < int32_t(x[rs2])){
                PC -= 4;
                PC += imm;
            }
            break;
        }

        case(0x5):{
            if(int32_t(x[rs1]) >= int32_t(x[rs2])){
                PC -= 4;
                PC += imm;
            }
            break;
        }

        case(0x6):{
            if(x[rs1] < x[rs2]){
                PC -= 4;
                PC += imm;
            }
            break;
        }

        case(0x7):{
            if(x[rs1] >= x[rs2]){
                PC -= 4;
                PC += imm;
            }
            break;
        }

        default:{
            trap(2, instruction);
        }
    }
}

void CPU::execJAL(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t imm = immJ(instruction);

    x[rd] = PC;
    PC = PC - 4 + imm;
}

void CPU::execJALR(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t funct3 = ::funct3(instruction);
    uint32_t rs1 = ::rs1(instruction);
    uint32_t imm = immI(instruction);

    if(funct3 == 0x0){
        uint32_t target = (x[rs1] + imm) & ~1u;
        x[rd] = PC;
        PC = target;
    } else {
        trap(2, instruction);
    }
}

void CPU::execLUI(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t imm = immU(instruction);

    x[rd] = imm;
}

void CPU::execAUIPC(uint32_t instruction){
    uint32_t rd = ::rd(instruction);
    uint32_t imm = immU(instruction);

    x[rd] = PC - 4 + imm;
}

void CPU::execEnv(uint32_t instruction){
    uint32_t funct3 = ::funct3(instruction);
    uint32_t imm = instruction >> 20;

    if (funct3 == 0){
        switch(imm){
            case(0x000):{
                trap(11, 0);
                break;
            }

            case(0x001):{
                stopped = true;
                break;
            }

            case(0x302):{
                PC = CSR[0x341];
                break;
            }

            case(0x105):{
                break;
            }

            default:{
                trap(2, instruction);
            }
        }
    } else {
        uint32_t rs1 = ::rs1(instruction);
        uint32_t rd = ::rd(instruction);
        uint32_t src = (funct3 & 0x4) ? rs1 : x[rs1];
        uint32_t buf = CSR[imm];


        switch(funct3 & 0x3){
            case(0x1):{
                CSR[imm] = src;
                break;
            }

            case(0x2):{
                CSR[imm] = CSR[imm] | src;
                break;
            }

            case(0x3):{
                CSR[imm] = CSR[imm] & ~src;
                break; 
            }

            default:{
                trap(2, instruction);
                return;
            }
        }
        x[rd] = buf;
    }
}

void CPU::cycle(){
    uint32_t instruction = bus->read32(PC);
    uint32_t opcode = instruction & 0x7F;

    PC += 4;

    switch(opcode){
        case (0b0110011): execOp(instruction); break;      //
        case (0b0010011): execOpImm(instruction); break;   //
        case (0b0000011): execLoad(instruction); break;    //
        case (0b0100011): execStore(instruction); break;   //
        case (0b1100011): execBranch(instruction); break;  //
        case (0b1101111): execJAL(instruction); break;     //
        case (0b1100111): execJALR(instruction); break;    //
        case (0b0110111): execLUI(instruction); break;     //
        case (0b0010111): execAUIPC(instruction); break;   //
        case (0b1110011): execEnv(instruction); break;     //opcode 0x73
        case (0b0001111): break;
        default: trap(2, instruction);
    }

    x[0] = 0;
}

void CPU::dump() const {
    printf("PC = %08X\n", PC);
    for (int i = 0; i < 32; ++i) {
        printf("x%-2d = %08X%s", i, x[i], (i % 4 == 3) ? "\n" : "   ");
    }
}

uint32_t CPU::getReg(int number){
    return x[number];
}