#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "graph.h"
#include "parser.h"
#include <math.h>
#include <limits.h>


#define MAX_ROW 999
#define MAX_COLUMN 18278

int R,C;
int is_disabled = 0;
int ** grid;
Graph* graph;
Coordinates* parser(char* cmd, int* oper, int row, int col);


void generate_grid(int r, int c) {
    grid = (int **)malloc((r+1) * sizeof(int *));
    if (grid == NULL) {
        exit(1);
    }

    for (int i = 0; i <= r; i++) {
        grid[i] = (int *)calloc((c+1), sizeof(int));
        if (grid[i] == NULL) {
            exit(1);
        }
        // for (int j = 0; j <= c; j++) {
        //     grid[i][j] = 0;
        // }
    }
    // Create nodes for the graph
    // for (int i=1; i<=r;i++){
    //     for (int j=1; j<=c;j++){
    //         Coordinates v1 = {-1 , -1};
    //         Coordinates v2 = {-1, -1};
    //         int  func;
    //         func = -1;
    //         addNode(graph, &func, &v1, &v2, i, j);
    //     }
    // }
    // access a cell by graph->matrix[row][col]
}

void column_decoder(char cc[], int j){
    int cnt = 0;
    while(j > 0){
            j--;
            int rem = (j % 26);
            cc[cnt] = 'A' + rem;
            j /= 26;
            cnt++;
    }
    cc[cnt] = '\0';
    // reverse the string
    for(int i = 0;i<(cnt/2);i++){
            char temp = cc[i];
    cc[i] = cc[cnt - i - 1];
    cc[cnt - i - 1] = temp;
    }
}

int min(int a, int b){
    int ans = (a > b) ? b:a;
    return ans;
}

int max(int a, int b){
    int ans = (a < b) ? b:a;
    return ans;
}

void print_grid(int start_x, int start_y, int r, int c){
    int max_x = min(9+start_x, r);
    int max_y = min(9+start_y, c);
    // print the first row containing the column alphabet
    for(int i = start_x - 1;i<=max_x;i++){
            for(int j = start_y - 1;j<=max_y;j++){
                    if(i == start_x - 1 && j == start_y - 1){
                            printf("%12c",' ');
                            continue;
                    }
                    else if(i == start_x - 1 && j != start_y - 1){
                            // first row
                            // print columns
                            char cc[] = "   ";
                            column_decoder(cc,j);
                            printf("%12s",cc);
                    }
                    else if(j == start_y - 1){
                            printf("%12d",i);
                    }
                    else{
                        if(graph->matrix[i][j] == 0){
                            printf("           0");
                        }
                        else if(graph->matrix[i][j]->valid){
                            printf("%12d", grid[i][j]);
                        }
                        else{
                            // printf("asffd%d,%d\n",i,j);
                            printf("%12s", "ERR");
                        }
                        // printf("%12d", grid[i][j]);
                    }
            }
            printf("\n");
    }

}

void display_status(int x, double time_taken){
    // display time and status of the previous command
    printf("[%.2f] ", time_taken);
    if(x == 1){
        printf("(ok) > ");
    }
    else if(x == 2){
        printf("(invalid range) > ");
    }
    else if(x == 3){
        printf("(unrecognized cmd) > ");
    }
    else if(x == 4){
        printf("(invalid row/column) > ");
    }
    else if(x == 5){
        printf("(cycle not allowed) > ");
    }
    else if(x == 6){
        printf("(invalid range) > ");
    }
    fflush(stdout);
}

int is_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

void adding_edges(int flag, int function, Coordinates* target, int value1_x, int value2_x, int value1_y, int value2_y){
    Coordinates v1 = {-1 , -1};
    Coordinates v2 = {-1, -1};
    int  func;
    func = -1;
    if(graph->matrix[target->row][target->col] == 0){
        // printf("Node is not present"); //debug
        addNode(graph, &func, &v1, &v2, target->row, target->col);
    }
    if(flag){
        if(value1_y==-1 && value2_y == -1){ //V V
            return; //if new values are constants, return
        }
        else if(value1_y==-1){ //V C
            if(graph->matrix[value2_x][value2_y] == 0){
                // printf("Node is not present"); //debug
                addNode(graph, &func, &v1, &v2, value2_x, value2_y);
            }
            addEdge(graph->matrix[value2_x][value2_y], target);
            return;
        }
        else if(value2_y==-1){ //C V
            if(graph->matrix[value1_x][value1_y] == 0){
                // printf("Node is not present"); //debug
                addNode(graph, &func, &v1, &v2, value1_x, value1_y);
            }
            addEdge(graph->matrix[value1_x][value1_y], target);
            return;
        }
        if(function == 1 || function == 2 || function == 3 || function == 4 || function == 5){ //C C Range
            for(int r=value1_x; r<=value2_x; r++){
                for(int c=value1_y; c<=value2_y;c++){
                    if(graph->matrix[r][c] == 0){
                        // printf("Node is not present"); //debug
                        addNode(graph, &func, &v1, &v2, r, c);
                    }
                    addEdge(graph->matrix[r][c], target);
                }
            }
        }
        else{ //C C Non-range
            if(graph->matrix[value1_x][value1_y] == 0){
                // printf("Node is not present"); //debug
                addNode(graph, &func, &v1, &v2, value1_x, value1_y);
            }
            if(graph->matrix[value2_x][value2_y] == 0){
                // printf("Node is not present"); //debug
                addNode(graph, &func, &v1, &v2, value2_x, value2_y);
            }
            addEdge(graph->matrix[value1_x][value1_y], target);
            addEdge(graph->matrix[value2_x][value2_y], target);
        }
    }
    else{
        int row_start=graph->matrix[target->row][target->col]->value1.row;
        int row_end=graph->matrix[target->row][target->col]->value2.row;
        int col_start=graph->matrix[target->row][target->col]->value1.col;
        int col_end=graph->matrix[target->row][target->col]->value2.col;
        if(col_start==-1 && col_end == -1){ //V V
            return; //if new values are constants, return
        }
        else if(col_start==-1){ //C V
            addEdge(graph->matrix[row_end][col_end], target);
            return;
        }
        else if(col_end==-1){ //V C
            addEdge(graph->matrix[row_start][col_start], target);
            return;
        }
        if(function == 1 || function == 2 || function == 3 || function == 4 || function == 5){ //C C Range
            for(int r=row_start; r<=row_end; r++){
                for(int c=col_start; c<=col_end;c++){
                    addEdge(graph->matrix[r][c], target);
                }
            }
        }
        else{ //C C Non-range
            addEdge(graph->matrix[row_start][col_start], target);
            addEdge(graph->matrix[row_end][col_end], target);
        }
    }
}

void break_edges(int flag, int function,  Coordinates* target, int value1_x, int value2_x, int value1_y, int value2_y){
    if(graph->matrix[target->row][target->col] == 0){ //Checker for empty cell
        return;
    }
    if(flag){ //Case of removing new edges
        if(value1_y==-1){ // V C or V V
            if (value2_y <= 0) { //V V
                return;
            }
            removeEdge(graph->matrix[value2_x][value2_y], target);
            return;
        }
        else if(value2_y==-1){ // C V
            removeEdge(graph->matrix[value1_x][value1_y], target);
            return;
        }
        if(function == 1 || function == 2 || function == 3 || function == 4 || function == 5){ // C C Range
            for(int r=value1_x; r<=value2_x; r++){
                for(int c=value1_y; c<=value2_y;c++){
                    removeEdge(graph->matrix[r][c], target);
                }
            }
        }
        else{ // C C Non Range
            removeEdge(graph->matrix[value1_x][value1_y], target);
            removeEdge(graph->matrix[value2_x][value2_y], target);
        }
    }
    else{ //Case of removing old edges using data stored in value
        int row_start=graph->matrix[target->row][target->col]->value1.row;
        int row_end=graph->matrix[target->row][target->col]->value2.row;
        int col_start=graph->matrix[target->row][target->col]->value1.col;
        int col_end=graph->matrix[target->row][target->col]->value2.col;

        if(col_start==-1){ //V C
            if (col_end <= 0) {
                return; //V V
            }
            removeEdge(graph->matrix[row_end][col_end], target);
            return;
        }
        else if(col_end==-1){ //C V
            removeEdge(graph->matrix[row_start][col_start], target);
            return;
        }
        if(function == 1 || function == 2 || function == 3 || function == 4 || function == 5){ //Range C C
            for(int r=row_start; r<=row_end; r++){
                for(int c=col_start; c<=col_end;c++){
                    removeEdge(graph->matrix[r][c], target);
                }
            }
        }
        else{ //Non Range C C
            removeEdge(graph->matrix[row_start][col_start], target);
            removeEdge(graph->matrix[row_end][col_end], target);
        }
    }
}


int getting_things_updated(int function, Coordinates* target, Coordinates* value1, Coordinates* value2, int r, int c){
    /*
    Encoding:
    0- constant
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
    -1- ERROR
    */
    
    //Made new Links
    //Start
    // printf("Starting break edges"); //debug
    
    // check the range first
    // printf("function: %d\n", function); //debug
    // in range functions
    int value1_x = value1->row;
    int value1_y = value1->col;
    int value2_x = value2->row;
    int value2_y = value2->col;
    int target_row = target->row;
    int target_column = target->col;

    // printf("value1_x: %d value1_y: %d value2_x: %d value2_y: %d\n", value1_x, value1_y, value2_x, value2_y); //debug
    // printf("target_row: %d target_col : %d\n", target_row, target_column);

    if(function == 1 || function == 2 || function == 3 || function == 4 || function == 5){
        if(value1_y > value2_y && value1_x != -1 && value2_x != -1 && value2_y != -1 && value1_y != -1){
            return 6;
        }
        if(value1_x > value2_x && value1_x != -1 && value2_x != -1 && value2_y != -1 && value1_y != -1){
            return 6;
        }
    }
    // clock_t start = clock(); //debug
    break_edges(0, function, target, value1_x, value2_x, value1_y, value2_y); //previous
    adding_edges(1, function, target, value1_x, value2_x,value1_y, value2_y); //new
    
    int numSortedNodes = 0;
    // printf("Starting topo"); //debug

    // here i am assuming that the bounds for target_row and target_column are already checked somewhere 
    // call hascycleutil here
    // if cycle then some topo will be there by kahns algo and use that to reset the nodes
    // else if no cycle then topo will be there by kahns algo and use that to update the nodes
    
    // call hascycle to check if there is any cycle or not
    // if they belong to the same connected component 

    // check if has cycle
    if(hasCycle(graph,graph->matrix[target_row][target_column],&r,&c)){
        break_edges(1, function, target, value1_x, value2_x, value1_y, value2_y); //new
        adding_edges(0, function, target, value1_x, value2_x, value1_y, value2_y); //previous
        return 5;
    }

    Vector* topo = topological(graph, graph->matrix[target_row][target_column],&r,&c, &numSortedNodes);

    graph->matrix[target_row][target_column]->function = function;
    graph->matrix[target_row][target_column]->value1.row = value1_x;
    graph->matrix[target_row][target_column]->value1.col = value1_y;
    graph->matrix[target_row][target_column]->value2.row = value2_x;
    graph->matrix[target_row][target_column]->value2.col = value2_y;

    if (topo != NULL) {
        for (int i = 0; i< numSortedNodes; i++) {
            Node* p = graph->matrix[topo->data[i].row][topo->data[i].col];
            // printf("%d %d\n", p->position.row, p->position.col); //debug
            if(p==NULL){
                break;
            }
            // p->visited = false;
            // p->inStack = false;
            // printf("Topo sort %d %d %d %d\n", p->position.row, p->position.col, p->function, function); //debug
            // printf("Topo sort %d %d %d %d\n", p->value1.row, p->value1.col, p->value2.row, p->value2.col); //debug
            int curr_row = p->position.row;
            int curr_col = p->position.col;
            if (p->value1.row == -1 && p->value2.row == -1 && p->value1.col ==-1 &&  p->value2.col == -1){
                p->value1.row = value1_x;
                p->value2.row = value2_x;
                p->value1.col = value1_y;
                p->value2.col = value2_y;
            }
            //max
            if (p->function == 2) {
                // Find max among directly connected outward edges
                int max_val = INT_MIN;
                int flag = 0;
                for(int k=p->value1.row; k<=p->value2.row; k++){
                    for(int j=p->value1.col; j<=p->value2.col;j++){
                        if(graph->matrix[k][j]->valid == false){
                            flag = 1;
                            // grid[curr_row][curr_col] = false;
                        }
                        else{
                            // grid[curr_row][curr_col] = true;
                            max_val = max(max_val, grid[k][j]);
                        }
                    }
                }
                if(flag){
                    graph->matrix[curr_row][curr_col]->valid = false;
                    // grid[curr_row][curr_col] = 0;
                }
                else{
                    graph->matrix[curr_row][curr_col]->valid = true;
                    grid[curr_row][curr_col] = max_val;
                }
                // grid[curr_row][curr_col] = min_val;
                // printf("max_val: %d\n", max_val); //debug
            }
            //min
            else if (p->function == 1) {
                // Find min among directly connected outward edges
                int min_val = INT_MAX;
                int flag = 0;
                for(int k=p->value1.row; k<=p->value2.row; k++){
                    for(int j=p->value1.col; j<=p->value2.col;j++){
                        if(graph->matrix[k][j]->valid == false){
                            flag = 1;
                            // grid[curr_row][curr_col] = false;
                        }
                        else{
                            // grid[curr_row][curr_col] = true;
                            min_val = min(min_val, grid[k][j]);
                        }
                    }
                }
                if(flag){
                    graph->matrix[curr_row][curr_col]->valid = false;
                    // grid[curr_row][curr_col] = 0;
                }
                else{
                    graph->matrix[curr_row][curr_col]->valid = true;
                    grid[curr_row][curr_col] = min_val;
                }
                grid[curr_row][curr_col] = min_val;
            }
            //avg
            else if (p->function == 4) {
                // graph->matrix[curr_row][curr_col]->valid = true;
                // Calculate average of directly connected outward edges
                int sum = 0;
                int count = 0;
                int flag = 0;
                for(int k=p->value1.row; k<=p->value2.row; k++){
                    for(int j=p->value1.col; j<=p->value2.col;j++){
                        if(graph->matrix[k][j]->valid == false){
                            flag = 1;
                            // graph->matrix[curr_row][curr_col]->valid = false;
                        }
                        else{
                            // grid[curr_row][curr_col] = true;
                            sum += grid[k][j];
                        }
                        count++;
                    }
                }
                if(flag){
                    graph->matrix[curr_row][curr_col]->valid = false;
                    // grid[curr_row][curr_col] = 0;
                }
                else{
                    graph->matrix[curr_row][curr_col]->valid = true;
                    grid[curr_row][curr_col] = (count > 0) ? sum / count : 0;
                }
                // grid[curr_row][curr_col] = (count > 0) ? sum / count : 0;
            }
            //sum
            else if (p->function==3) {
                // graph->matrix[curr_row][curr_col]->valid = true;
                // Sum of directly connected outward edges
                int sum = 0;
                int flag = 0;
                for(int k=p->value1.row; k<=p->value2.row; k++){
                    for(int j=p->value1.col; j<=p->value2.col;j++){
                        // if any cell in the grid is invalid then the cell is invalid
                        if(graph->matrix[k][j]->valid == false){
                            flag = 1;
                            // graph->matrix[curr_row][curr_col]->valid = false;
                        }
                        else{
                            // grid[curr_row][curr_col] = true;
                            sum += grid[k][j];
                        }
                    }
                }
                if(flag){
                    graph->matrix[curr_row][curr_col]->valid = false;
                    // grid[curr_row][curr_col] = 0;
                }
                else{
                    graph->matrix[curr_row][curr_col]->valid = true;
                    grid[curr_row][curr_col] = sum;
                }
            }
            //const
            else if (p->function==0) {
                if(p->value1.col==-1){
                    graph->matrix[curr_row][curr_col]->valid = true;
                    grid[curr_row][curr_col] = p->value1.row;
                }
                else{
                    // graph->matrix[curr_row][curr_col]->valid = true;
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false){
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col];
                    }
                    // grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col];
                }
            }
            //sleep
            else if (p->function==6) {
                
                // printf("reached sleep\n"); //debug
                // Sleep for the given number of seconds
                // graph->matrix[curr_row][curr_col]->valid = true;
                // printf("p->value1.col: %d\n", p->value1.col); //debug
                if(p->value1.col==-1){
                    if(p->value1.row < 0){
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = p->value1.row;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        sleep(p->value1.row);
                        grid[curr_row][curr_col] = p->value1.row;
                    }
                }
                else{
                    if(grid[p->value1.row][p->value1.col] < 0){
                        if(graph->matrix[p->value1.row][p->value1.col]->valid == false){
                            // grid[curr_row][curr_col] = 0;
                            graph->matrix[curr_row][curr_col]->valid = false;
                        }
                        else{
                            graph->matrix[curr_row][curr_col]->valid = true;
                            grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col];
                        }
                        // grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col];
                    }
                    else{
                        if(graph->matrix[p->value1.row][p->value1.col]->valid == false){
                            graph->matrix[curr_row][curr_col]->valid = false;
                        }
                        else{
                            graph->matrix[curr_row][curr_col]->valid = true;
                            sleep(grid[p->value1.row][p->value1.col]);
                            grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col];
                        }
                    }
                }
                
            }
            //sub
            else if(p->function == 8){
                // graph->matrix[curr_row][curr_col]->valid = true;
                if(p->value1.col==-1 && p->value2.col == -1){
                    grid[curr_row][curr_col] = p->value1.row - p->value2.row;
                    graph->matrix[curr_row][curr_col]->valid = true;
                }
                else if(p->value1.col==-1){
                    if(graph->matrix[p->value2.row][p->value2.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = p->value1.row - grid[p->value2.row][p->value2.col];
                    }
                    // grid[curr_row][curr_col] = p->value1.row - grid[p->value2.row][p->value2.col];
                }
                else if(p->value2.col==-1){
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] =  grid[p->value1.row][p->value1.col] - p->value2.row;
                    }
                    // grid[curr_row][curr_col] =  grid[p->value1.row][p->value1.col] - p->value2.row;
                }
                else{
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false || graph->matrix[p->value2.row][p->value2.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] - grid[p->value2.row][p->value2.col];
                    }
                    // grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] - grid[p->value2.row][p->value2.col];
                }
            }
            //add
            else if(p->function == 7){
                // graph->matrix[curr_row][curr_col]->valid = true;
                if(p->value1.col==-1 && p->value2.col == -1){
                    graph->matrix[curr_row][curr_col]->valid = true;
                    grid[curr_row][curr_col] = p->value1.row + p->value2.row;
                }
                else if(p->value1.col==-1){
                    if(graph->matrix[p->value2.row][p->value2.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value2.row][p->value2.col] + p->value1.row;
                    }
                    // grid[curr_row][curr_col] = grid[p->value2.row][p->value2.col] + p->value1.row;
                }
                else if(p->value2.col==-1){
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = p->value2.row + grid[p->value1.row][p->value1.col];
                    }
                    // grid[curr_row][curr_col] = p->value2.row + grid[p->value1.row][p->value1.col];
                }
                else{
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false || graph->matrix[p->value2.row][p->value2.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] + grid[p->value2.row][p->value2.col];
                    }
                    // grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] + grid[p->value2.row][p->value2.col];
                }
            }
            //multiply
            else if(p->function == 9){
                // graph->matrix[curr_row][curr_col]->valid = true;
                // printf("p->value1.col: %d\n", p->value1.col); //debug
                // printf("p->value2.col: %d\n", p->value2.col); //debug
                if(p->value1.col==-1 && p->value2.col == -1){
                    grid[curr_row][curr_col] = p->value1.row * p->value2.row;
                    graph->matrix[curr_row][curr_col]->valid = true;
                }
                else if(p->value1.col==-1){
                    if(graph->matrix[p->value2.row][p->value2.col]->valid == false){
                        // grid[curr_row][curr_col] = 0;
                        graph->matrix[curr_row][curr_col]->valid = false;
                        // printf("dddd111\n");
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value2.row][p->value2.col] * p->value1.row;
                    }
                    // grid[curr_row][curr_col] = grid[p->value2.row][p->value2.col] * p->value1.row;
                }
                else if(p->value2.col==-1){
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false){
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = p->value2.row * grid[p->value1.row][p->value1.col];
                    }
                    // grid[curr_row][curr_col] = p->value2.row * grid[p->value1.row][p->value1.col];
                }
                else{
                    if(graph->matrix[p->value1.row][p->value1.col]->valid == false || graph->matrix[p->value2.row][p->value2.col]->valid == false){
                        graph->matrix[curr_row][curr_col]->valid = false;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] * grid[p->value2.row][p->value2.col];
                    }
                    // grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] * grid[p->value2.row][p->value2.col];
                }
            }
            //divide
            else if(p->function == 10){
                //Raise errors for division by zero
                // printf("%d, %d, %d, %d\n", p->value1.row, p->value1.col, p->value2.row, p->value2.col); //debug
                int flag = 0;
                // printf("reached here\n"); //debug
                if(p->value1.col==-1 && p->value2.col == -1){
                    if(p->value2.row == 0){
                        // printf("dddd1\n");
                        flag = 1;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = p->value1.row / p->value2.row;
                    }
                }
                    
                else if(p->value1.col==-1){
                    if(grid[p->value2.row][p->value2.col] == 0){
                        // 1/A1
                        // printf("dddd2\n");
                        flag = 1;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = p->value1.row / grid[p->value2.row][p->value2.col];
                    }
                    
                }
                else if(p->value2.col==-1){
                    // A1/2
                    if(p->value2.row == 0){
                        // printf("dddd3\n");
                        flag = 1;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] / p->value2.row ;
                    }
                }
                else{
                    // A4/A3
                    if(grid[p->value2.row][p->value2.col] == 0){
                        // printf("dddd4\n");
                        flag = 1;
                    }
                    else{
                        graph->matrix[curr_row][curr_col]->valid = true;
                        grid[curr_row][curr_col] = grid[p->value1.row][p->value1.col] / grid[p->value2.row][p->value2.col];
                    }
                }
                if(flag){
                    // printf("dddd5\n");
                    int nx = 0;
                    graph->matrix[curr_row][curr_col]->valid = false;
                    Vector* topo1 = topological(graph, graph->matrix[curr_row][curr_col],&r,&c,&nx); 
                    for(int j=0;j<nx;j++){
                        Node* pn = graph->matrix[topo1->data[i].row][topo1->data[i].col];
                        // printf("node in topo1: (%d,%d)\n",topo1[j]->position.row, topo1[j]->position.col); //debug
                        graph->matrix[pn->position.row][pn->position.col]->valid = false;
                    }
                    // grid[curr_row][curr_col] = 0;
                    // printf("\n"); //debug
                }
            }
            //stdev
            /////////////////////////////////////////////////////////////////////////////////////////////
            else if(p->function == 5){
                // Calculate standard deviation of directly connected outward edges
                int sum = 0;
                int count = 0;
                for(int k=p->value1.row; k<=p->value2.row; k++){
                    for(int j=p->value1.col; j<=p->value2.col;j++){
                        sum += grid[k][j];
                        count++;
                    }
                }
                double mean = (count > 0) ? sum / count : 0;
                double stdev = 0;
                int flag = 0;
                for(int k=p->value1.row; k<=p->value2.row; k++){
                    for(int j=p->value1.col; j<=p->value2.col;j++){
                        if(graph->matrix[k][j]->valid == false){
                            flag = 1;
                            // grid[curr_row][curr_col] = false;
                            break;
                        }
                        else{
                            // grid[curr_row][curr_col] = true;
                            stdev += ((grid[k][j] - mean) * (grid[k][j] - mean));
                        }
                    }
                    if(flag) break;
                }
                if(flag){
                    graph->matrix[curr_row][curr_col]->valid = false;
                    // grid[curr_row][curr_col] = 0;
                }
                else{
                    graph->matrix[curr_row][curr_col]->valid = true;
                    if(count==0) grid[curr_row][curr_col] = 0;
                    else {
                        stdev = sqrt(stdev / count);
                        grid[curr_row][curr_col] = (int)round(stdev);
                    }
                }
                
            }
            else {
                return 3;
            }
        }
    }
    // printf("Topo sort %d\n", numSortedNodes); //debug
    freeVector(topo);
    return 1;
}

int process_command(char command[], int* start_x, int* start_y, const int r, const int c){
    // 1 - successful execution
    // 2 - invalid range
    // 3 - unrecognized cmd
    // 4 - invalid row/column
    // q - quit
    // printf("In process command\n"); //debug
    if(strcmp(command, "q") == 0){
        return 0;  // Exit program
    }
    // shifting commands
    if(strcmp(command, "w") == 0){
        if(*start_x > 10){
            *start_x = *start_x - 10;
        }
        else{
            // 1 2 3 4 5 6 7 8 9
            *start_x = 1;
        }
        //*start_x = (*start_x > 10) ? *start_x - 10 : *start_x;
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        return 1;
    }

    if(strcmp(command, "s") == 0){
        if(*start_x + 10 <= r){
            *start_x = *start_x+10;
            *start_x = min(*start_x, r-9);
        }
        // *start_x = (*start_x + 10 <= r) ? *start_x + 10 :*start_x;
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        return 1;
    }

    if(strcmp(command, "a") == 0){
        if(*start_y > 10){
            *start_y = *start_y - 10;
        }
        else{
            *start_y = 1;
        }
        //*start_y = (*start_y > 10) ? *start_y - 10 : *start_y;
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        return 1;
    }

    if(strcmp(command, "d") == 0){
        if(*start_y + 10 <= c){
            *start_y = *start_y+10;
            *start_y = min(*start_y, c-9);
        }
        // *start_y = (*start_y + 10 <= c) ? *start_y + 10 : *start_y;
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        return 1;
    }

    Coordinates* coord;
    int function; //if function = 3 fail
    coord = parser(command, &function, r, c);
    //printf("function: %d\n", function); //debug
    if(function == 12){
        is_disabled = 1;
        return 1;
    }
    if(function == 11){
        is_disabled = 0;
        return 1;
    }
    //for example
    // printf("target_row: %d, target_column: %d\n", target_row, target_column);
    // printf("value1_x: %d value1_y: %d value2_x: %d value2_y: %d\n", value1_x, value1_y, value2_x, value2_y); //debug
    if(function == 13){
        // change start_x start_y and print grid
        *start_x = coord[0].row;
        *start_y = coord[0].col;
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        return 1;
    }
    if(function == -1){
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        free(coord);
        return 3;
    }
    else{
        // printf("Starting processing"); //debug
        int status = 3;
        status = getting_things_updated(function, &coord[0], &coord[1], &coord[2], r, c);
        if(!is_disabled){
            print_grid(*start_x,*start_y,r,c);
        }
        free(coord);
        return status;
    }
    return 3;
}

int process_first(int x,char* command[], int* start_x, int* start_y){
    // invalid number of commands
    if(x != 3){
        return 0;
    }
    // check type of commands
    if (!is_number(command[1]) || !is_number(command[2])) {
        return 0;
    }
    // check if the rows and columns are in the valid range
    char *endptr;
    long r = strtol(command[1], &endptr, 10);
    long c = strtol(command[2], &endptr, 10);
    if (r < 1 || r > MAX_ROW) {
        return 0;
    }

    if (c < 1 || c > MAX_COLUMN) {
        return 0;
    }
    clock_t start = clock();
    graph=createGraph(r+1,c+1);
    generate_grid(r,c);
    // print grid only when enabled
    if(!is_disabled){
        print_grid(*start_x,*start_y,r,c);
    }
    clock_t end = clock();
    double time_taken = (double)(end - start)/CLOCKS_PER_SEC;
    display_status(1,time_taken);
    return 1;
}


int main(int argc, char *argv[]) {
    int start_x = 1;
    int start_y = 1;
    if(process_first(argc,argv,&start_x,&start_y) == 0){
        return 1;
    }
    int r = atoi(argv[1]);
    int c = atoi(argv[2]);
    R = r;
    C = c;
    char *command = NULL;  // Pointer for dynamic input
    size_t len = 0;

    while(1){
        ssize_t read = getline(&command, &len, stdin);
        if (read == -1) {
            break;  // Exit loop if input fails (e.g., Ctrl+D)
        }
        if (read > 0 && command[read - 1] == '\n') {
                command[read - 1] = '\0';
        }
        if (strlen(command) == 0) {
                printf("[0.0] (unrecognized cmd) > ");
                continue;
        }
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        int status = process_command(command, &start_x, &start_y, r, c);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
        // quit status
        if (status == 0) {
            break;
        }
        display_status(status,time_taken);
    }
    free(command);
    //free grid
    for (int i = 0; i <= r; i++) {
        free(grid[i]);
    }
    free(grid);
    return 0;
}

