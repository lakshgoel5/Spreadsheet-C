#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include "coordinate.h"

typedef struct {
    Coordinates* table;
    int cur_size;
    int capacity;
} HashTable;

int hashfn(Coordinates* value, int capacity);
void init_table(HashTable *ht);
void insert(HashTable *ht, Coordinates* value);
void deleting(HashTable *ht, Coordinates* value);
void rehash(HashTable *ht);

#endif