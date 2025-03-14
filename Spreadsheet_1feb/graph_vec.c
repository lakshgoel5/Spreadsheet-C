#include "graph_vec.h"

Graph* createGraph(int rows, int cols){
    if (rows > MAX_ROWS + 1 || cols > MAX_COLS + 1) return NULL;
    
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    // Allocate array of row pointers
    graph->matrix = (Node***)malloc((rows + 1) * sizeof(Node**)); //check r+1
    if (!graph->matrix) {
        free(graph);
        return NULL;
    }
    
    // Allocate each row
    for (int i = 0; i <= rows; i++) {
        graph->matrix[i] = (Node**)calloc(cols + 1, sizeof(Node*)); //check c+1
        if (!graph->matrix[i]) {
            // Cleanup previously allocated rows
            for (int j = 0; j < i; j++) {
                free(graph->matrix[j]);
            }
            free(graph->matrix);
            free(graph);
            return NULL;
        }
    }
    graph->rows = rows;
    graph->cols = cols;
    // graph->nodeCount = 0;
    
    return graph;
}

void addNode(Graph* graph, const int* function, Coordinates* value1, Coordinates* value2, int row, int col){
    if (row > graph->rows || col > graph->cols || row <= 0 || col <= 0) {
        return;
    }
    
    if (graph->matrix[row][col] != NULL) {
        return;  // Node already exists
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    
    Vector* v;
    v=createVector(1);
    newNode->function = *function;
    newNode->value1.row = value1->row;
    newNode->value1.col = value1->col;
    newNode->value2.row = value2->row;
    newNode->value2.col = value2->col;
    newNode->position.row = row;
    newNode->position.col = col;
    // newNode->edges = NULL;
    newNode->v = v;
    newNode->visited = false;
    newNode->inStack = false;
    newNode->valid = true;
    
    graph->matrix[row][col] = newNode;
    return;
}

void addEdge(Node* node, Coordinates* target){
    // printf("came on add edge1: %d %d\n", node->position.row, node->position.col);
    // printf("came on add edge2: %d %d\n", target->row, target->col);
    pushBack(node->v ,target);
    return;
}

void removeEdge(Node* node, Coordinates* target){
    deleting_v(node->v, target);
    return;
}

bool hasCycle_util(Graph* graph, Node* node, bool* visited, bool* instack, int* r, int* c){
    Vector* v = node -> v;
    visited[(node->position.row-1) * (*c)+(node->position.col-1)] = true;
    instack[(node->position.row-1) * (*c)+(node->position.col-1)] = true;
    // printf("doing dfs of: %d %d\n", node->position.row, node->position.col);
    for(int i=0; i < v->size; i++ ){
        if(instack[(v->data[i].row-1) * (*c)+(v->data[i].col-1)]){
            return true;
        }
        if(!visited[(v->data[i].row-1) * (*c)+(v->data[i].col-1)]){
            if(hasCycle_util(graph, graph->matrix[v->data[i].row][v->data[i].col], visited, instack, r , c)){
                return true;
            }
        }
    }
    instack[(node->position.row-1) * (*c)+(node->position.col-1)] = false;
    return false;
}

bool hasCycle(Graph* graph, Node* node, int* r, int* c){
    // printf("came in cycle\n");
    bool* visited = (bool*)calloc((*r) * (*c), sizeof(bool));
    bool* instack = (bool*)calloc(((*r) * (*c)), sizeof(bool)); //vector

    bool ans = hasCycle_util(graph, node, visited, instack, r, c);
    free(visited);
    free(instack);
    // if(ans){ //debug
    //     printf("Cycle detected!!!!\n");
    // }
    return ans;
}

void topological_util(Graph* graph, Node* node, Node **result, bool* visited, int* index, int* r, int* c){
    Vector* v = node -> v;
    visited[(node->position.row-1) * (*c)+(node->position.col-1)] = true;
    result[*index] = node;
    (*index)++;
    for(int i=0; i < v->size; i++ ){
        if(!visited[(v->data[i].row-1) * (*c)+(v->data[i].col-1)]){
            topological_util(graph, graph->matrix[v->data[i].row][v->data[i].col], result, visited, index, r , c);
        }
        
    }
}

Node** topological(Graph* graph, Node* node, int* r, int* c, int* num){
    bool* visited = (bool*)calloc((*r) * (*c), sizeof(bool));
    Node** result = (Node**)malloc(((*r) * (*c)) * sizeof(Node*)); //vector
    int index = 0;
    topological_util(graph, node, result, visited, &index, r, c);
    *num=index;
    free(visited);
    return result;
}

void freeGraph(Graph* graph){
    for (int i = 1; i <= graph->rows; i++) {
        for (int j = 1; j <= graph->cols; j++) {
            if (graph->matrix[i][j]) {
                freeVector(graph->matrix[i][j]->v);
                free(graph->matrix[i][j]);
            }
        }
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph);
}