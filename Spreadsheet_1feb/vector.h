#include <stdio.h>
#include <stdlib.h>
#include "coordinate.h"

typedef struct {
    Coordinates *data;
    int size;
    int capacity;
} Vector;

Vector* createVector(int initialCapacity);
void resizeVector(Vector* vector);
void pushBack(Vector* vector, Coordinates* value);
void deleting_v(Vector* vector, Coordinates* value);
// Coordinates get(Vector* vector, int index);
void freeVector(Vector* vector);