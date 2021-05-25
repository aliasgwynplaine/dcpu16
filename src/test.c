#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "dcpu16.h"
#include "hash.h"

int main() {
    struct _hash_t ht;
    hash_init(&ht, 60);
    hash_insert(&ht, "SET", SET);
    hash_insert(&ht, "ADD", ADD);
    hash_insert(&ht, "SUB", SUB);
    hash_insert(&ht, "MUL", MUL);
    hash_insert(&ht, "MLI", MLI);
    hash_insert(&ht, "DIV", DIV);
    hash_insert(&ht, "DVI", DVI);
    hash_insert(&ht, "MOD", MOD);
    hash_insert(&ht, "MDI", MDI);
    hash_insert(&ht, "AND", AND);
    hash_insert(&ht, "BOR", BOR);
    hash_insert(&ht, "XOR", XOR);
    hash_insert(&ht, "SHR", SHR);
    hash_insert(&ht, "ASR", ASR);
    hash_insert(&ht, "SHL", SHL);
    hash_insert(&ht, "ADX", ADX);
    hash_insert(&ht, "SBX", SBX);
    hash_insert(&ht, "STI", STI);
    hash_insert(&ht, "STD", STD);
    hash_insert(&ht, "JSR", JSR);
    hash_insert(&ht, "INT", INT);
    hash_insert(&ht, "IAG", IAG);
    hash_insert(&ht, "IAS", IAS);
    hash_insert(&ht, "RFI", RFI);
    hash_insert(&ht, "IAQ", IAQ);
    hash_insert(&ht, "HWN", HWN);
    hash_insert(&ht, "HWQ", HWQ);
    hash_insert(&ht, "IFB", IFB);
    hash_insert(&ht, "IFC", IFC);
    hash_insert(&ht, "IFE", IFE);
    hash_insert(&ht, "IFN", IFN);
    hash_insert(&ht, "IFG", IFG);
    hash_insert(&ht, "IFA", IFA);
    hash_insert(&ht, "IFL", IFL);
    hash_insert(&ht, "IFU", IFU);
    hash_insert(&ht, "HWI", HWI);
    hash_insert(&ht, "A", A);
    hash_insert(&ht, "B", B);
    hash_insert(&ht, "C", C);
    hash_insert(&ht, "X", X);
    hash_insert(&ht, "Y", Y);
    hash_insert(&ht, "Z", Z);
    hash_insert(&ht, "I", I);
    hash_insert(&ht, "J", J);
    hash_insert(&ht, "PUSH", PUSHPOP);
    hash_insert(&ht, "POP", PUSHPOP);
    hash_insert(&ht, "SP", SP);
    hash_insert(&ht, "PEEK", PEEK);
    hash_insert(&ht, "PICK", PICK);
    hash_insert(&ht, "PC", PC);
    hash_insert(&ht, "EX", EX);

    printhashtable(&ht);
    return 0;
}
