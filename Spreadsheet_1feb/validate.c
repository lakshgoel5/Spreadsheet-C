#include "validate.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <string.h>

bool isNumOrLet(char c) { //true if character is a number or letter
    return (((c<='Z' && c>='A') || (c<='9' && c>='0') || (c<='z' && c>='a')) ? true : false);
}

bool isSymbol(char c) {
    return (c=='+' || c=='-' || c=='*' || c=='/' || c=='(' || c==')' || c==':' || c=='=' || c=='_' || c==' ') ? true : false;
}

bool isCell(char* str) {
    if (strlen(str)==0) return false;
    int i=0;
    bool num = false;
    bool let = false;
    for( ; i<3 ; i++) {
        if(str[i]<='Z' && str[i]>='A') {let=true; continue;}
        else break;
    }
    for(int j=0 ; j<3 ; i++, j++) {
        if(str[i]<='9' && str[i]>='0') {num=true; continue;}
        else break;
    }
    if (str[i]=='\0') return let&&num;
    return false;
}

bool isNum (char* str) {
    if (strlen(str)==0) return false;
    char* endptr;
    strtol(str, &endptr, 10);
    if (*endptr=='\0') return true;
    return false;
}
bool isValue(char* str) {
    if (strlen(str)==0) return false;
    return (isCell(str) || isNum(str));
}

bool Validity(char* command) {
    char* *brokenList = malloc(10*sizeof(char*)); //list to contain broken parts of the cmd
    int j = 0; //index for brokenList
    char cmd[50];
    strcpy(cmd, command); //copy of the command
    int start = 0;
    for(int i=0 ; cmd[start]!='\0' && j<11 ; i++) {
        if(isNumOrLet(cmd[i])) continue;
        if(isSymbol(cmd[start]) && start==i) { //single character-symbol
            brokenList[j] = malloc(2*sizeof(char));
            brokenList[j][0] = cmd[start];
            brokenList[j][1] = '\0';
            j++;
            start++;
        }
        else if(isNumOrLet(cmd[start])) {
            int len = i-start+1; //+1 for '\0'
            brokenList[j] = malloc(len*sizeof(char));
            for(int k=start ; k<i ; k++) {
                brokenList[j][k-start] = cmd[k];
            }
            brokenList[j][len-1] = '\0';
            start = i;
            j++;
            i--;
        }
        else if(cmd[i]=='\0') {
            brokenList[j] = malloc(2*sizeof(char));
            brokenList[j][0] = cmd[i];
            brokenList[j][1] = '\0';
            j++;
        }
        else {
            return false;
        }
    }
    if(j>=10) return false;
    //By this point, we have the entire cmd broken into parts around symbols
    //Now we need to check if the parts are valid
    //Case 1: enable/disable_output
    if (j==3) {
        if((strcmp(brokenList[0], "enable")==0 || strcmp(brokenList[0], "disable")==0) && strcmp(brokenList[1], "_")==0 && strcmp(brokenList[2], "output")==0) return true;
    }

    //Case 2: Scroll_to cell
    if(j==5) {
        if(isCell(brokenList[4]) && strcmp(brokenList[0], "scroll")==0 && strcmp(brokenList[1], "_")==0 && strcmp(brokenList[2], "to")==0 && strcmp(brokenList[3], " ")==0) return true;
    }

    //Case 3: expression
    if (!isCell(brokenList[0])) return false;
    if (strcmp(brokenList[1], "=") != 0) return false;
    // After the first two strings, we decide from the number of strings
    switch (j) {
        case 3 : //we expect cell=value
            if (isValue(brokenList[2])) return true;
            break;
        case 4 : //we expect cell=+value or cell=-value
            if ((strcmp(brokenList[2], "+") == 0 || strcmp(brokenList[2], "-") == 0) && (isValue(brokenList[3]))) return true;
            break;
        case 5 : //we expect cell=value+-/*value
            if ((isValue(brokenList[2])) && ((strcmp(brokenList[3], "+")==0 || strcmp(brokenList[3], "-")==0 || strcmp(brokenList[3], "*")==0 || strcmp(brokenList[3], "/")==0)) && (isValue(brokenList[4])))  return true;
            break;
            case 6 : //we expect cell=SLEEP(value) OR cell=+-value+-/*value OR cell=value+-/*+-value
            if ((strcmp(brokenList[2], "SLEEP")==0) && (strcmp(brokenList[3],"(")==0) && isValue(brokenList[4]) && (strcmp(brokenList[5],")")==0) ) return true;
 	    else if ((isValue(brokenList[2])) && ((strcmp(brokenList[3], "+")==0 || strcmp(brokenList[3], "-")==0 || strcmp(brokenList[3], "*")==0 || strcmp(brokenList[3], "/")==0)) && (strcmp(brokenList[4],"+")==0 || (strcmp(brokenList[4],"-")==0)) && (isNum(brokenList[5]))) return true;
      	    else if (((strcmp(brokenList[2],"+")==0 || (strcmp(brokenList[2],"-")==0)) && isNum(brokenList[3])) && ((strcmp(brokenList[4], "+")==0 || strcmp(brokenList[4], "-")==0 || strcmp(brokenList[4], "*")==0 || strcmp(brokenList[4], "/")==0)) && (isValue(brokenList[5]))) return true;
   break;
        case 7 : //we expect cell=SLEEP(-value) OR cell=+-value+-/*+-value
            if ((strcmp(brokenList[2], "SLEEP") == 0) && strcmp(brokenList[3], "(")==0 && (strcmp(brokenList[4],"+")==0 || strcmp(brokenList[4],"-")==0) && isValue(brokenList[5]) && strcmp(brokenList[6],")")==0 ) return true;
      	    else if ((strcmp(brokenList[2],"+")==0 || (strcmp(brokenList[2],"-")==0)) && isNum(brokenList[3]) && (strcmp(brokenList[4], "+")==0 || strcmp(brokenList[4], "-")==0 || strcmp(brokenList[4], "*")==0 || strcmp(brokenList[4], "/")==0) && (strcmp(brokenList[5],"+")==0 || strcmp(brokenList[5],"-")==0) && (isNum(brokenList[6]))) return true;
            break;
        case 8 : //we expect cell=func(cell:cell)
            if (((strcmp(brokenList[2], "MAX")==0 || strcmp(brokenList[2],"MIN")==0 || strcmp(brokenList[2],"AVG")==0 || strcmp(brokenList[2],"SUM")==0 || strcmp(brokenList[2],"STDEV")==0)) && strcmp(brokenList[3],"(")==0 && isCell(brokenList[4]) && strcmp(brokenList[5],":")==0 && isCell(brokenList[6]) && strcmp(brokenList[7],")")==0 ) return true;
            break;
        default : return false;
    }
    return false;
}
