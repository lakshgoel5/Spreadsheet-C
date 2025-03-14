#ifndef VALIDATE_H
#define VALIDATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"

bool isNumOrLet(char c);
bool isSymbol(char c);
bool isCell(char* str);
bool isNum (char* str);
bool isValue(char* str);
bool Validity(char* command);

#endif