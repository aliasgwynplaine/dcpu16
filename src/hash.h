#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

struct _hash_t {
    char ** key;
    int * value;
    int sz;
};

void hash_init(struct _hash_t * h, int sz) {
    h->sz = sz;
    h->key = (char**) malloc(sz * sizeof(char *));
    h->value = (int *) malloc(sz * sizeof(int));
}

u32 _index(struct _hash_t * h, char * key) {
    u32 i = 0;
    u32 j = (uintptr_t) key % h->sz;

    while (h->key[j] && h->key[j] != key) {
        j = (j + i) % h->sz;
        
        if (i == h->sz) {
            fprintf(stderr, "hash table overflow!\n");
            return -1;
        }

        i++;
    }

    return j;
}

int hash_insert(struct _hash_t * h, char * key, int value) {
    u32 i = _index(h, key);
    
    if (i == -1) return -1;
    
    h->key[i] = key;
    h->value[i] = value;

    return 0;
}

int hash_get_value(struct _hash_t * h, char * key) {
    u32 i = _index(h, key);
    return h->value[i];
}

void printhashtable(struct _hash_t * h) {
    for (int i = 0; i < h->sz; i++) {
        if (h->key[i])
            printf(
                "%s -> %d\n", h->key[i], hash_get_value(h, h->key[i])
            );
    }
}

#endif