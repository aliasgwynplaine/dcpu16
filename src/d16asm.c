#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "dcpu16.h"
/*

here i load opcodes

*/



u16 instr2bin(char * instr) {
    /*
    
    "OP B, A" must be translated into 
    aaaaaabbbbbooooo; a,b,c are bits

    */
    u16 bincode;
    /* extract opcode from instruction */
    u8 aa, bb, oo;
    // extract A
    // extract B
    bincode = (aa<<10) + (bb<<5) + oo;
    return bincode;
}

int main(int argc, char * argv[]) {
    if (argc == 3) {
        u16 * mem = (u16 *) malloc(0x10000 * sizeof(u16));

        if (mem == NULL) {
            fprintf(stderr, "No memory!\n");
            exit(1);
        }

        memset(mem, 0x0, 0x10000);
        debug("[*] Memory initialized!");
        debug("[*] Attempting to translate %s", argv[1]);
        //translated
        u32 words = 0;

        if (words > 0x10000) {
            dbgerr( 
                "[!] We're out of memory!!. Try some smaller program"
            );
            exit(2);
        }

        // export file
        debug("[*] Program assembled! Creating bin file");
        debug("[*] %s was created!", argv[2]);
        debug("[*] Completed!");

    } else {
        fprintf(stderr, "usage: %s <src_file> <output_file>\n", argv[0]);
    }
    
    return 0;
}