#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include "coordinate.h"
#include "hash.h"
#include "vector.h"
#define MAX_ROWS 999
#define MAX_COLS 18278

// Structure for a graph node (cell)
typedef struct Node {
    int function;
    Coordinates value1;
    Coordinates value2;
    Coordinates position;
    // struct Edge* edges;  // Linked list of edges
    HashTable* ht;
    // bool visited;        // For cycle detection and topological sort
    // bool inStack;        // For cycle detection
    bool valid;          // For checking if the node is valid
} Node;

typedef struct {
    Node*** matrix;  // 2D matrix of nodes
    int rows;
    int cols;
   // int nodeCount;
} Graph;

Graph* createGraph(int rows, int cols);
void addNode(Graph* graph, const int* function, Coordinates* value1, Coordinates* value2, int row, int col);
void addEdge(Node* node, Coordinates* target);
void removeEdge(Node* node, Coordinates* target);
bool hasCycle(Graph* graph, Node* node, int* r, int* c);
bool hasCycle_util(Graph* graph, Node* node, bool* visited, bool* instack, int* r, int* c);
Vector* topological(Graph* graph, Node* node, int* r, int* c, int* num);
void freeGraph(Graph* graph);
#endif