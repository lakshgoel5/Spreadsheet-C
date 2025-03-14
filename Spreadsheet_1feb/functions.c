#include "functions.h"


int max(int** grid, int* value1_x, int* value1_y, int* value2_x, int* value2_y){
    int max_val = INT_MIN;
        int flag = 0;
        for(int k=topo[i]->value1.row; k<=topo[i]->value2.row; k++){
            for(int j=topo[i]->value1.col; j<=topo[i]->value2.col;j++){
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