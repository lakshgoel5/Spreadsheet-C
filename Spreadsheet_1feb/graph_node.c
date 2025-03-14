#include "graph_node.h"

// Function to create a new node
Node* createNode(const int* function, Coordinates* value1, Coordinates* value2, int row, int col) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return NULL;
    
    newNode->function = *function;
    newNode->value1.row = value1->row;
    newNode->value1.col = value1->col;
    newNode->value2.row = value2->row;
    newNode->value2.col = value2->col;
    newNode->position.row = row;
    newNode->position.col = col;
    newNode->edges = NULL;
    newNode->visited = false;
    newNode->inStack = false;
    newNode->valid = true;
    
    return newNode;
}

// Function to create a new graph
Graph* createGraph(int rows, int cols) {
    if (rows > MAX_ROWS + 1 || cols > MAX_COLS + 1) return NULL;
    
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    // Allocate array of row pointers
    graph->matrix = (Node***)malloc((rows + 1) * sizeof(Node**));
    if (!graph->matrix) {
        free(graph);
        return NULL;
    }
    
    // Allocate each row
    for (int i = 0; i <= rows; i++) {
        graph->matrix[i] = (Node**)calloc(cols + 1, sizeof(Node*));
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
    graph->nodeCount = 0;
    
    return graph;
}

// Function to add a node to the graph
bool addNode(Graph* graph, const int* function, Coordinates* value1, Coordinates* value2, int row, int col) {
    if (row > graph->rows || col > graph->cols || row <= 0 || col <= 0) {
        // printf("ddd");
        return false;
    }
    
    if (graph->matrix[row][col] != NULL) {
        // printf("ddd1111\n");
        return false;  // Node already exists
    }
    
    Node* newNode = createNode(function, value1, value2, row, col);
    if (!newNode) return false;
    
    graph->matrix[row][col] = newNode;
    graph->nodeCount++;
    
    return true;
}
// check if duplicate edges are added or not - no duplicate edges
bool addEdge(Graph* graph, int fromRow, int fromCol, int toRow, int toCol) {
    // 1-based indexing
    if (fromRow > graph->rows || fromCol > graph->cols || 
        toRow > graph->rows || toCol > graph->cols ||
        fromRow <= 0 || fromCol <= 0 || toRow <= 0 || toCol <= 0) {
        return false;
    }
    
    Node* source = graph->matrix[fromRow][fromCol];
    Node* dest = graph->matrix[toRow][toCol];
    
    if (!source || !dest) return false;
    
    // check if already an edge exists between the two nodes
    Edge* current = source->edges; 
    while (current != NULL) { 
        if (current->destination == dest) { 
            return false;  // already edge exists hence reject
        }
        current = current->next;
    }
    // Create new edge
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    if (!newEdge) return false;
    
    newEdge->destination = dest;
    newEdge->next = source->edges;
    source->edges = newEdge;
    
    return true;
}

// Function to remove an edge between two nodes
bool removeEdge(Graph* graph, int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow >= graph->rows || fromCol >= graph->cols || 
        toRow >= graph->rows || toCol >= graph->cols ||
        fromRow < 0 || fromCol < 0 || toRow < 0 || toCol < 0) {
        return false;
    }
    
    Node* source = graph->matrix[fromRow][fromCol];
    if (!source) return false;
    
    Edge* current = source->edges;
    Edge* prev = NULL;
    
    while (current != NULL) {
        if (current->destination->position.row == toRow && 
            current->destination->position.col == toCol) {
            if (prev) {
                prev->next = current->next;
            } else {
                source->edges = current->next;
            }
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

// Helper function for cycle detection
bool hasCycleUtil(Node* node) {
    if (!node) return false;
    // here if it returns true then there is a cycle with the new introduction
    if (!node->visited) {
        node->visited = true;
        node->inStack = true;
        
        Edge* edge = node->edges;
        while (edge != NULL) {
            if (!edge->destination->visited && hasCycleUtil(edge->destination)) {
                return true;
            } else if (edge->destination->inStack) {
                return true;
            }
            edge = edge->next;
        }
    }
    node->inStack = false;
    return false;
}

// Function to detect cycles in the graph
bool hasCycle(Graph* graph) {
    // Check for cycles from each node
    for (int i = 1; i <= graph->rows; i++) {
        for (int j = 1; j <= graph->cols; j++) {
            if (graph->matrix[i][j] && !graph->matrix[i][j]->visited) {
                if (hasCycleUtil(graph->matrix[i][j])) {
                    return true;
                }
            }
        }
    }
    for (int i = 1; i <= graph->rows; i++) {
        for (int j = 1; j <= graph->cols; j++) {
            if (graph->matrix[i][j]) {
                graph->matrix[i][j]->visited = false;
                graph->matrix[i][j]->inStack = false;
            }
        }
    }
    return false;
}

// Helper function for topological sort
// Helper function for topological sort
void topologicalSortUtil(Node* node, Node** sortedNodes, int* index) {
    if (!node || node->visited) return;

    // printf("Visiting node (%d, %d)\n", node->position.row, node->position.col); //debug
    node->visited = true;

    // Recursively visit all connected nodes
    Edge* edge = node->edges;
    while (edge != NULL) {
        if (!edge->destination->visited) {
            topologicalSortUtil(edge->destination, sortedNodes, index);
        }
        edge = edge->next;
    }

    sortedNodes[*index] = node; // Store node in reverse postorder
    (*index)--;
}


// Enum for topological sort strategy
    
// Function to perform topological sort
Node** topologicalSort(Graph* graph, int startRow, int startCol, int* numSortedNodes) {
    if (startRow <= 0 || startRow > graph->rows ||
        startCol <= 0 || startCol > graph->cols ||
        !graph->matrix[startRow][startCol]) {
        // printf("ERROR: Invalid start node (%d, %d)\n", startRow, startCol);
        // printf("xxxx\n");
        return NULL;
    }

    // printf("DEBUG: Starting Topo Sort from (%d, %d)\n", startRow, startCol);

    // Reset visited status for all nodes

    // remove this
    // first time no need

    // for (int i = 1; i <= graph->rows; i++) {
    //     for (int j = 1; j <= graph->cols; j++) {
    //         if (graph->matrix[i][j]) {
    //             graph->matrix[i][j]->visited = false;
    //             graph->matrix[i][j]->inStack = false;
    //         }
    //     }
    // }

    // Check for cycles only in reachable subgraph
    if (hasCycleUtil(graph->matrix[startRow][startCol])) {
        // printf("ERROR: Cycle detected in reachable subgraph. Sorting not possible.\n");
        // printf("yyyy\n");
        return NULL;
    }

    // Reset visited flags again for topological sort
    for (int i = 1; i <= graph->rows; i++) {
        for (int j = 1; j <= graph->cols; j++) {
            if (graph->matrix[i][j]) {
                graph->matrix[i][j]->visited = false;
                graph->matrix[i][j]->inStack = false;
            }
        }
    }

    // Allocate space for temporary sorted nodes


    // printf("DEBUG: Number of nodes: %d\n", graph->nodeCount); //debug

    Node** tempSortedNodes = (Node**)malloc(graph->nodeCount * sizeof(Node*));
    if (!tempSortedNodes) return NULL;

    int index = graph->nodeCount - 1;
    // printf("index: %d\n",index);
    // printf("DEBUG: Calling DFS from (%d, %d)\n", startRow, startCol);
    topologicalSortUtil(graph->matrix[startRow][startCol], tempSortedNodes, &index);

    // Calculate number of sorted nodes
    *numSortedNodes = graph->nodeCount - index - 1;
    // printf("DEBUG: Number of sorted nodes: %d\n", *numSortedNodes);

    if (*numSortedNodes == 0) {
        // printf("ERROR: No nodes were sorted!\n");
        free(tempSortedNodes);
        return NULL;
    }

    // Create final array of exact size needed

    // printf("DEBUG: Allocating space for %d sorted nodes\n", *numSortedNodes); //debug

    Node** finalSortedNodes = (Node**)malloc((*numSortedNodes+1) * sizeof(Node*)); //mistake

    // printf("DEBUG: Allocated space for sorted nodes\n"); //debug

    if (!finalSortedNodes) {
        free(tempSortedNodes);
        return NULL;
    }

    // printf("DEBUG: Copying sorted nodes\n"); //debug


    // Copy only the valid nodes
    for (int i = 0; i < *numSortedNodes; i++) {
        finalSortedNodes[i] = tempSortedNodes[index + 1 + i];
    }
    finalSortedNodes[*numSortedNodes] = NULL;  // Mark end of array

    free(tempSortedNodes);
    return finalSortedNodes;
}

// Function to free the graph
void freeGraph(Graph* graph) {
    for (int i = 1; i <= graph->rows; i++) {
        for (int j = 1; j <= graph->cols; j++) {
            if (graph->matrix[i][j]) {
                Edge* currentEdge = graph->matrix[i][j]->edges;
                while (currentEdge != NULL) {
                    Edge* nextEdge = currentEdge->next;
                    free(currentEdge);
                    currentEdge = nextEdge;
                }
                
                // free(graph->matrix[i][j].function);
                // free(graph->matrix[i][j]->value1);
                // free(graph->matrix[i][j]->value2);
                free(graph->matrix[i][j]);
            }
        }
    }
    
    free(graph);
}

