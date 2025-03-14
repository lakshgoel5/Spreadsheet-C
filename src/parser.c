// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "parser.h"

// int row=100, col=100;
// typedef struct {
//         int row;
//         int col;
//     } Coordinates;
//Comment these out later while adding and also change the imports to just parser.h

Coordinates strtocell(char *cell, int row, int col, int* error) {
    Coordinates a;
    if (cell[0]<='9' && cell[0]>='0') { //if it is a constant, then convert directly
            a.row = atoi(cell); //For uniformity, store the value in row, and a dummy value in col
            a.col = -1;
            return a;
    }
    if (cell[0]=='+') {
	Coordinates a = strtocell(cell+1, row, col, error);
	return a;
    }
    if (cell[0]=='-') {
	Coordinates a = strtocell(cell+1, row, col, error);
	a.row*=(-1);
	return a;
    }
    char* ptr = cell;
    a.col = 0;
    while(*ptr<='Z' && *ptr>='A')
    {
            a.col = a.col * 26 + *ptr - 'A' + 1;
            ptr++;
    }
    a.row = atoi(ptr);
    if(a.row<=row && a.col<=col) {
        return a;
    }
    *error = 1; //if invalid cell coordinates, flag error
    return a;
}

bool charFinder(char* str, char c) {
        char* str1 = (char*)malloc(strlen(str)+1);

        strcpy(str1, str);
        while(*str1 != '\0') {
                if (*str1 == c) return true;
                str1++;
        }
        return false;
}


Coordinates* parser(char *cmd, int *oper, int row, int col) {
        if(!Validity(cmd)) { //checking for validity of command
                Coordinates *a = (Coordinates *)malloc(3 * sizeof(Coordinates));
                *oper = -1;
                return a;
        }
        if(!charFinder(cmd, '=')) {
                if(!charFinder(cmd, ' ')) { //enable/disable_output
                        if(cmd[0]=='e') *oper = 11;
                        else *oper = 12;
                        Coordinates *a = (Coordinates *)malloc(3 * sizeof(Coordinates));
                        return a;
                }
                else { //scroll_to cell
                        int error = 0; //to check for errors mid processing
                        char* equal = strchr(cmd, ' ');
                        char* exp = equal+1;
                        Coordinates* a = (Coordinates *)malloc(3*sizeof(Coordinates));
                        a[0] = strtocell(exp, row, col, &error);
                        if(error) {*oper = -1; return a;}
                        *oper = 13;
                        return a;
                }
        }
    
    int error = 0; //to check for errors mid processing

    char* equal = strchr(cmd, '=');
    *equal = '\0';
    char* cell;
    char* exp;
    cell = cmd;
    exp = equal + 1;

    Coordinates *a = (Coordinates *)malloc(3 * sizeof(Coordinates));
    a[0] = strtocell(cell, row, col, &error);
    if(error) {*oper = -1; return a;}
    if (strncmp(exp, "MIN(", 4)==0) {
            *oper = 1;
            exp+=4;
            size_t len = strlen(exp);
            exp[len-1] = '\0';
            equal = strchr(exp, ':');
            *equal = '\0';
            a[1] = strtocell(exp, row, col, &error);
            if(error) {*oper = -1; return a;}
            a[2] = strtocell(equal+1, row, col, &error);
            if(error) {*oper = -1; return a;}
    }
    else if (strncmp(exp, "MAX(", 4)==0) {
            *oper = 2;
            exp+=4;
            size_t len = strlen(exp);
            exp[len-1] = '\0';
            equal = strchr(exp, ':');
            *equal = '\0';
            a[1] = strtocell(exp, row, col, &error);
            if(error) {*oper = -1; return a;}
            a[2] = strtocell(equal+1, row, col, &error);
            if(error) {*oper = -1; return a;}
    }
    else if (strncmp(exp, "SUM(", 4)==0) {
            *oper = 3;
            exp+=4;
            size_t len = strlen(exp);
            exp[len-1] = '\0';
            equal = strchr(exp, ':');
            *equal = '\0';
            a[1] = strtocell(exp, row, col, &error);
            if(error) {*oper = -1; return a;}
            a[2] = strtocell(equal+1, row, col, &error);
            if(error) {*oper = -1; return a;}
    }
    else if (strncmp(exp, "AVG(", 4)==0) {
            *oper = 4;
            exp+=4;
            size_t len = strlen(exp);
            exp[len-1] = '\0';
            equal = strchr(exp, ':');
            *equal = '\0';
            a[1] = strtocell(exp, row, col, &error);
            if(error) {*oper = -1; return a;}
            a[2] = strtocell(equal+1, row, col, &error);
            if(error) {*oper = -1; return a;}
    }
    else if (strncmp(exp, "STDE", 4)==0){ //std deviation

        *oper = 5;
        exp+=6;
        size_t len = strlen(exp);
        exp[len-1] = '\0';
        equal = strchr(exp, ':');
        *equal = '\0';
        a[1] = strtocell(exp, row, col, &error);
        if(error) {*oper = -1; return a;}
        a[2] = strtocell(equal+1, row, col, &error);
        if(error) {*oper = -1; return a;}
}
else if (strncmp(exp, "SLEE", 4)==0) { //sleep
        exp+=6;
        bool negative = false;
        if(exp[0]=='-') {exp++; negative=true;} //if -value, then equivalent to value*-1
        *oper = 6;
        size_t len = strlen(exp);
        exp[len-1] = '\0';
        a[1] = strtocell(exp, row, col, &error);
        if(error) {*oper = -1; return a;}
        a[2].row = a[2].col = -1;
        if (negative) a[1].row *= -1;
}
else { //basic operations
        char* restexp = exp+1; //one added before check to include instructions of the kind '=-A1' or '=+A1'
        char* operator = strpbrk(restexp, "+-*/");
        if (operator != NULL) {
                switch (*operator) {
                        case '+':
                                *oper = 7;
                                break;
                        case '-':
                                *oper = 8;
                                break;
                        case '*':
                                *oper = 9;
                                break;
                        case '/':
                                *oper = 10;
                                break;
                }
                *operator = '\0';
                a[1] = strtocell(exp, row, col, &error);
                if(error) {*oper = -1; return a;}
                a[2] = strtocell(operator + 1, row, col, &error);
                if(error) {*oper = -1; return a;}
        } else {
                *oper = 0; // no operation
                if (exp[0] == '-') { //if exp[0]=='-', then it is same as 0-exp, hence *oper=8(subtraction)
                    a[1] = strtocell("0", row, col, &error);
                    if(error) {*oper = -1; return a;}
                    a[2] = strtocell(exp+1, row, col, &error);
                    if(error) {*oper = -1; return a;}
                    *oper = 8;
                }
                else if (exp[0] == '+') { //if exp[0]=='+', then we need to begin with exp+1 while deducing the value
                    a[1] = strtocell(exp+1, row, col, &error);
                    if(error) {*oper = -1; return a;}
                    a[2].row = a[2].col = -1;
                }
                else {
                    a[1] = strtocell(exp, row, col, &error);
                    if(error) {*oper = -1; return a;}
                    a[2].row = a[2].col = -1;
                }
        }
}
return a;
}

/*
Encoding:
0- CONSTANT ASSIGNMENT
1- MIN
2- MAX
3- SUM
4- AVG
5- STDEV
6- SLEEP
7- ADD
8- SUB
9- MUL
10- DIV
11- ENABLE
12-DISABLE
13- SCROLL_TO
-1- ERROR
*/
