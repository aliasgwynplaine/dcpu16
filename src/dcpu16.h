#ifndef _DCPU16_H
#define _DCPU16_H

#include "utils.h"

enum operat {
    XXX, SET, ADD, SUB, MUL, MLI, DIV, DVI, MOD, MDI,
    AND, BOR, XOR, SHR, ASR, SHL, IFB, IFC, IFE, IFN,
    IFG, IFA, IFL, IFU, ADX=0x1a, SBX, STI=0x1e, STD 
};

enum special_operat {
    XXY, JSR, INT=0x08, IAG, IAS, 
    RFI, IAQ, HWN=0x10, HWQ, HWI 
};

enum reg_index {
    A   ,B  ,  C,  X,  Y,  Z,  I,  J,
    p_A=0x8, p_B, p_C, p_X, p_Y, p_Z, p_I, p_J,
    l_A=0x10, l_B, l_C, l_X, l_Y, l_Z, l_I, l_J,
    PUSHPOP=0x18, PEEK, PICK, SP, PC, EX, RNW, LNW
};

struct clock {
    u64 cycles;
};

struct dcpu16 { // 0x1000c regs in total
    u16 r[8];
    u16 pc;
    u16 sp;
    u16 ex;
    u16 ia;
    u16 m[0x10000];
};



#endif