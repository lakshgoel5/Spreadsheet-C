#include "graph.h"

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
    
    HashTable *ht;
    ht=(HashTable*)malloc(sizeof(HashTable));
    newNode->function = *function;
    newNode->value1.row = value1->row;
    newNode->value1.col = value1->col;
    newNode->value2.row = value2->row;
    newNode->value2.col = value2->col;
    newNode->position.row = row;
    newNode->position.col = col;
    // newNode->edges = NULL;
    init_table(ht);
    newNode->ht = ht;
    // newNode->visited = false;
    // newNode->inStack = false;
    newNode->valid = true;
    
    graph->matrix[row][col] = newNode;
    return;
}

void addEdge(Node* node, Coordinates* target){
    // printf("came on add edge1: %d %d\n", node->position.row, node->position.col);
    // printf("came on add edge2: %d %d\n", target->row, target->col);
    insert(node->ht,target);
    return;
}

void removeEdge(Node* node, Coordinates* target){
    deleting(node->ht, target);
    return;
}

bool hasCycle_util(Graph* graph, Node* node, bool* visited, bool* instack, int* r, int* c){
    HashTable* ht = node -> ht;
    visited[(node->position.row-1) * (*c)+(node->position.col-1)] = true;
    instack[(node->position.row-1) * (*c)+(node->position.col-1)] = true;
    // printf("doing dfs of: %d %d\n", node->position.row, node->position.col);
    for(int i=0; i < ht->capacity; i++ ){
        // printf("neighbour1: %d %d\n", ht->table[i].row, ht->table[i].col);
        if((ht->table[i].col != -1  && ht->table[i].row != -1) && (ht->table[i].col != -2  && ht->table[i].row != -2)){
            // printf("neighbour: %d %d\n", ht->table[i].row, ht->table[i].col);
            if(instack[(ht->table[i].row-1) * (*c)+(ht->table[i].col-1)]){
                return true;
            }
            if(!visited[(ht->table[i].row-1) * (*c)+(ht->table[i].col-1)]){
                if(hasCycle_util(graph, graph->matrix[ht->table[i].row][ht->table[i].col], visited, instack, r , c)){
                    return true;
                }
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

void topological_util(Graph* graph, Node* node, Vector *result, bool* visited, int* index, int* r, int* c){
    HashTable* ht = node -> ht;
    visited[(node->position.row-1) * (*c)+(node->position.col-1)] = true;
    pushBack(result,&(node->position));
    (*index)++;
    for(int i=0; i < ht->capacity; i++ ){
        // printf("neighbour11: %d %d\n", ht->table[i].row, ht->table[i].col);
        if((ht->table[i].col != -1  && ht->table[i].row != -1) && (ht->table[i].col != -2  && ht->table[i].row != -2)){
            // printf("neighbour12: %d %d\n", ht->table[i].row, ht->table[i].col);
            if(!visited[(ht->table[i].row-1) * (*c)+(ht->table[i].col-1)]){
                topological_util(graph, graph->matrix[ht->table[i].row][ht->table[i].col], result, visited, index, r , c);
            }
        }
    }
}

Vector* topological(Graph* graph, Node* node, int* r, int* c, int* num){
    bool* visited = (bool*)calloc((*r) * (*c), sizeof(bool));
    // Coordinates** result = (Coordinates**)malloc(((*r) * (*c)) * sizeof(Coordinates*)); //vector
    Vector* result = createVector(1);
    // int index=0;
    // result[0] = node;
    int index = 0;
    // printf("node row and col: %d %d\n",node->position.row, node->position.col);
    // printf("translates to: %d\n",(node->position.row-1) * (*c)+(node->position.col-1));
    topological_util(graph, node, result, visited, &index, r, c);
    *num=index;
    free(visited);
    return result;
}

void freeGraph(Graph* graph){
    for (int i = 1; i <= graph->rows; i++) {
        for (int j = 1; j <= graph->cols; j++) {
            if (graph->matrix[i][j]) {
                free(graph->matrix[i][j]->ht);
                free(graph->matrix[i][j]);
            }
        }
    }
    free(graph);
}