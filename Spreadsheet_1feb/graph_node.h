#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "coordinate.h"

#define MAX_STRING_LENGTH 100
#define MAX_ROWS 999
#define MAX_COLS 18278

// Structure for a graph node (cell)
typedef struct Node {
    int function;
    Coordinates value1;
    Coordinates value2;
    Coordinates position;
    struct Edge* edges;  // Linked list of edges
    bool visited;        // For cycle detection and topological sort
    bool inStack;        // For cycle detection
    bool valid;          // For checking if the node is valid
} Node;

// Structure for storing edges
typedef struct Edge {
    Node* destination;
    struct Edge* next;
} Edge;

// Structure for the graph
typedef struct {
    Node*** matrix;  // 2D matrix of nodes
    int rows;
    int cols;
    int nodeCount;
} Graph;

Node* createNode(const int* function, Coordinates* value1, Coordinates* value2, int row, int col);
Graph* createGraph(int rows, int cols);
bool addNode(Graph* graph, const int* function, Coordinates* value1, Coordinates* value2, int row, int col);
bool addEdge(Graph* graph, int fromRow, int fromCol, int toRow, int toCol);
bool removeEdge(Graph* graph, int fromRow, int fromCol, int toRow, int toCol);
bool hasCycleUtil(Node* node);
bool hasCycle(Graph* graph);
void topologicalSortUtil(Node* node, Node** sortedNodes, int* index);
Node** topologicalSort(Graph* graph, int startRow, int startCol, int* numSortedNodes);
void freeGraph(Graph* graph);
#endif

