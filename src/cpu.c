#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>
#include "debug.h"
#include "dcpu16.h"

/*

    dcpu16 emulator v1.7
 
*/
void dcpu16_init(struct dcpu16 * cpu) {
    memset(cpu, 0x0, 0x1000c * 0x2); // 0x10000 + 0x8 + 0x4
}

int printmem(struct dcpu16 cpu) {
    u32 i;
    printf("\n[!] RAM:");

    for (i = 0; i < sizeof(cpu.m) / sizeof(u16); i++) {
        if (i % 16 == 0) printf("\n%8.8x\t", i);
        printf("%4.4x ", cpu.m[i]);
    }

    printf("\n");

    return 0;
}


int printreg(struct dcpu16 cpu) {
    printf(
        "pc\tsp\tex\tia\ta\tb\tc\tx\ty\tz\ti\tj\n"
        "----------------------------------------------"
        "----------------------------------------------\n"
    );
    printf(
        "%4.4x\t%4.4x\t%4.4x\t%4.4x\t%4.4x\t%4.4x\t"
        "%4.4x\t%4.4x\t%4.4x\t%4.4x\t%4.4x\t%4.4x\n",
        cpu.pc, cpu.sp, cpu.ex, cpu.ia, cpu.r[A], cpu.r[B],
        cpu.r[C], cpu.r[X], cpu.r[Y], cpu.r[Z], cpu.r[I], 
        cpu.r[J]
    );
    return 0;
}


int load(struct dcpu16 * cpu) {
    /**
     * loads from stdin
    */
    u32 c = 0;
    //c = fread(cpu->m, 0x10000, 1, stdin);

    while (!feof(stdin))
        fread(cpu->m+(c++), sizeof(u16), 1, stdin);

    printf("[!] Loaded %d words!\n", c);
    return 0;
}


int cycle(u8 t) {
    /*
    * this function emulates the clock
    * may be useful in the future for input,
    * output and interruptions
    */
    return 0;
}


u16 * lookup(struct dcpu16 * cpu, u8 reg, bool skip, bool is_a) {
    u16 next_word; // may be necessary
    // todo: handle skip properly
    switch (reg) {
    case A : case B : case C : case X : 
    case Y : case Z : case I : case J : 
        cycle(0); return &cpu->r[reg];
    case p_A: case p_B: case p_C: case p_X:
    case p_Y: case p_Z: case p_I: case p_J:
        cycle(0);
        return &cpu->m[ cpu->r[reg&0x7] ];
    
    case l_A: case l_B: case l_C: case l_X: 
    case l_Y: case l_Z: case l_I: case l_J:
        cycle(1);
        next_word = cpu->m[ cpu->pc++ ];
        return &cpu->m[ cpu->r[reg&0x7] + next_word];
    
    case PUSHPOP:
        cycle(0);
        return is_a ? &cpu->m[cpu->sp++] : &cpu->m[--cpu->sp];
        break;

    case PEEK :
        cycle(0);
        return &cpu->m[ cpu->sp ];

    case PICK :
        cycle(1);
        next_word = cpu->m[ cpu->pc++ ];
        return &cpu->m[ cpu->sp + next_word];
    
    case SP : cycle(0); return &cpu->sp;
    case PC : cycle(0); return &cpu->pc;
    case EX : cycle(0); return &cpu->ex;

    case RNW :
        cycle(1); 
        return &cpu->m[ cpu->m[ cpu->pc++ ] ];
    
    case LNW :
        cycle(0); 
        return &cpu->m[ cpu->pc++ ];

    default : // todo: need some check
        cycle(0);
        u16 * retvalue;
        retvalue = (u16 *) malloc(sizeof(u16));
        *retvalue = ~(reg & 0x1f) + 1;
        debug("On default, returning %4.4x", *retvalue);
        return retvalue;
    }
}


int opr(struct dcpu16 *cpu, u16 code, bool skip) {
    debug("code: %x", code);
    u8 op = code & 0x1f;
    u8 bb = (code>>5) & 0x1f;
    u8 aa = (code>>10) & 0x3f;
    debug("op: %2.2x, bb: %2.2x, aa: %2.2x", op, bb, aa);
    u16 * a = lookup(cpu, aa, skip, true);
    u16 * b = lookup(cpu, bb, skip, false);
    u32 aux;
    s32 saux;
    debug("a: %4.4x", *a);
    debug("b: %4.4x", *b);

    if (skip) { // to handle IF concat
        if (op >= IFB && op <= IFU) opr(cpu, cpu->m[ cpu->pc++ ], true);
        return 0;
    }

    switch (op) {
    case XXX:
        switch(bb) {
            case XXY: break;
            case JSR: 
                //todo
                break;
        }
        break;
    case SET : *b = *a; cycle(1); break;
    case ADD : 
        aux = *b + *a;
        *b = aux & 0xffff;
        cpu->ex = aux>>16; // carry
        cycle(2);
        break;
    case SUB : 
        aux = *b - *a;
        *b = aux & 0xffff;
        cpu->ex = aux>>16; // carry
        cycle(2);
        break;
    case MUL :
        aux = (u32)*b * (*a);
        *b = aux & 0xffff;
        cpu->ex = (aux>>16) & 0xffff; // carry
        cycle(2);
        break;
    case MLI :
        saux = (s32) ((s16)*b * (s16)(*a));
        *b = saux & 0xffff;
        cpu->ex = (saux>>16) & 0xffff;
        cycle(2); 
        break;
    case DIV : 
        *b = *b / *a;
        cpu->ex = (*b<<16) / *a & 0xffff;
        cycle(3);
        break;
    case DVI :
        saux = (s16)*b / (s16)*a;
        *b = saux & 0xffff;
        cpu->ex = (saux<<16) / *a & 0xffff;
        cycle(3);
        break;
    case MOD : 
        *b = *a ? 0 : *b % *a;
        cycle(3);
        break;
    case MDI : 
        *b = *a ? 0 : (s16) *b % (s16) *a;
        cycle(3); 
        break;
    case AND : 
        *b = *b & *a;
        cycle(1);
        break;
    case BOR : 
        *b = *b | *a;
        cycle(1);
        break;
    case XOR : 
        *b = *b ^ *a;
        cycle(1);
        break;
    case SHR :
        aux = *b<<16;
        cpu->ex = ((aux)>>*a)&0xffff;
        *b = *b >> *a;
        cycle(1);
        break;
    case ASR : 
        aux = *b<<16;
        cpu->ex = ((aux)>>*a)&0xffff; 
        *b = ((s16)*b) >> *a;
        cycle(1);
        break;
    case SHL : 
        aux = *b<<*a;
        cpu->ex = ((aux)>>16)&0xffff;
        *b = *b<<*a;
        cycle(1);
        break;
    case IFB : 
        cycle(2);
        if (!(*b&*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case IFC :
        cycle(2);
        if (*b&*a) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case IFE : 
        cycle(2);
        if (!(*b==*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case IFN :
        cycle(2);
        if (!(*b!=*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case IFG :
        cycle(2);
        if (!(*b>*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
    case IFA : 
        cycle(2);
        if (!((s16)*b>(s16)*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case IFL :
        cycle(2);
        if (!(*b<*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case IFU : 
        cycle(2);
        if (!((s16)*b<(s16)*a)) opr(cpu, cpu->m[ cpu->pc++ ], true);
        break;
    case ADX : 
        aux = *b + *a + cpu->ex;
        *b = aux & 0xffff;
        cpu->ex = (aux>>16) & 0xffff;
        cycle(3);
        break;
    case SBX : 
        aux = *b - *a + cpu->ex;
        *b = aux & 0xffff;
        cpu->ex = (aux>>16) & 0xffff;
        cycle(3);
        break;
    case STI : 
        *b = *a;
        cpu->r[I]++, cpu->r[J]++;
        cycle(2);
        break;
    case STD :
        *b = *a;
        cpu->r[I]--, cpu->r[J]--;
        cycle(2);
        break;
    default:
        printf("[!] opcode %2.2x not suported", op); break;
    }

    return 0;
}


int run(struct dcpu16 *cpu) {
    while ( 1 ) { // 
        cycle(1);
        u16 code = cpu->m[cpu->pc++];
        opr(cpu, code, false);
    }
}


int main(int argc, char * argv[]) {
    struct dcpu16 cpu;
    debug("[*] Allocating memory...");
    //memset(&cpu, 0x0, 0x1000c * 0x2); // 0x10000 + 0x8 + 0x4
    dcpu16_init(&cpu);
    //printreg(cpu);
    //printmem(cpu);
    debug("[*] Memory allocated...");
    debug("[*] Loading program into ram...");
    load(&cpu);
    cpu.pc = 0x0;
    cpu.r[A] = 0xaa;
    cpu.r[B] = 0xbb;
    cpu.r[J] = 0x1;
    cpu.r[I] = 0x2;
    cpu.sp = 0xffff; // stack on the 'floor'
    debug("[*] Memory loaded!");
    printreg(cpu);
    opr(&cpu, 0x21, false);
    printreg(cpu);
    opr(&cpu, 0x22, false);
    printreg(cpu);
    opr(&cpu, 0x18e3, false);
    printreg(cpu);
    opr(&cpu, 0xfc61, false);
    printreg(cpu); 
    printmem(cpu);
    //run(cpu);
    debug("[*] Finishing main...");
    return 0;
}
