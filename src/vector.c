#include "vector.h"

Vector* createVector(int initialCapacity) {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (vector == NULL) {
        return NULL;
    }
    vector->data = (Coordinates*)malloc(initialCapacity * sizeof(Coordinates));
     if (vector->data == NULL) {
        free(vector);
        return NULL;
    }
    vector->size = 0;
    vector->capacity = initialCapacity;
    return vector;
}

void resizeVector(Vector* vector) {
    vector->capacity *= 2;
    Coordinates* newData = (Coordinates*)realloc(vector->data, vector->capacity * sizeof(Coordinates));
    if (newData == NULL) {
        return;
    }
    vector->data = newData;
}

void pushBack(Vector* vector, Coordinates* value) {
    if (vector->size == vector->capacity) {
        resizeVector(vector);
    }
    vector->data[vector->size].row = value->row;
    vector->data[vector->size].col = value->col;
    vector->size++;
}

void deleting_v(Vector* vector, Coordinates* value){
    int i;
    for(i=0;i < vector->size;i++){
        if(vector->data[i].row==value->row && vector->data[i].col==value->col){
            break;
        }
    }
    for(int j=i; j < vector->size-1; j++){
        vector->data[j].row=vector->data[j+1].row;
        vector->data[j].col=vector->data[j+1].col;
    }
    vector->size--;
}

void freeVector(Vector* vector) {
    free(vector->data);
    free(vector);
}