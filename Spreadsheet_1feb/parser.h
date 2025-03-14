#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "validate.h"
#include "graph.h"


Coordinates strtocell(char *cell, int row, int col, int* error);
bool charFinder(char* str, char c);
Coordinates* parser(char *cmd, int *oper, int row, int col);


#endif
