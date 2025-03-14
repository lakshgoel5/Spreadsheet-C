#include "hash.h"
#define ALPHA 0.7

void init_table(HashTable *ht){
    ht->capacity = 1;
    ht-> cur_size = 0;
    ht -> table = (Coordinates *)malloc(ht->capacity* sizeof(Coordinates));

    for(int i=0; i< ht->capacity; i++){
        ht->table[i].col = -1;
        ht->table[i].row = -1;
    }
}

int hashfn(Coordinates* value, int capacity){
    int x = value->row;
    int y = value->col;
    int ans = ( ( (x + y) * (x + y + 1) ) / 2 ) + y;
    return (ans % capacity);
}

void insert(HashTable *ht, Coordinates* value){
    // printf("inside hash insert\n");
    if ( ht->cur_size >= (ht->capacity * ALPHA)) {
        // printf("calling rehash\n");
        rehash(ht);
    }

    int index = hashfn(value, ht->capacity);
    // printf("index: %d", index);
    int original_index = index;
    //-1 for emply
    //-2 for deleted
    while((ht->table[index].col != -1  && ht->table[index].row != -1) &&
        (ht->table[index].col != -2  && ht->table[index].row != -2)){
        index = (index + 1) % ht->capacity;
        if (index == original_index) {
            printf("Table is full!\n");
            return;
        }
    }

    ht->table[index].col = value->col;
    ht->table[index].row = value->row;
    ht->cur_size++;
}

void deleting(HashTable *ht, Coordinates* value){
    int index = hashfn(value, ht->capacity);
    int original_index = index;

    while(ht->table[index].col != -1  && ht->table[index].row != -1){
        if(ht->table[index].col == value->col  && ht->table[index].row == value->row){
            ht->table[index].col = -2 ; 
            ht->table[index].row = -2 ;
            ht->cur_size--;
            return;
        }
        index = (index + 1) % ht->capacity;
        if (index == original_index) break;
    }
    printf("Not found node for deleting"); //debug
    return;
}

void rehash(HashTable *ht){
    int old_capacity = ht->capacity;
    Coordinates *old_table = ht->table;

    ht->capacity *= 2; // Double the size
    ht->cur_size = 0;
    ht->table = (Coordinates *)malloc(ht->capacity * sizeof(Coordinates)); //can use realloc

    for (int i = 0; i < ht->capacity; i++) {
        ht->table[i].row = -1;
        ht->table[i].col = -1;
    }

    for (int i = 0; i < old_capacity; i++) {
        if (old_table[i].row != -1 && old_table[i].col != -1 && old_table[i].col != -2  && old_table[i].row != -2) {
            insert(ht, &old_table[i]);
        }
    }

    free(old_table);
}