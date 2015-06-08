#ifndef GRAPH_H
#define GRAPH_H

typedef struct{
    int V, E;
    int *degree;
    int **adjList; /*the vertices are stored with their index (0 to |V|-1)*/
} Graph;

/** initializes a new graph with the values |V|, |E| and the array degree, then
    allocates enough space for adjList, so that it can be filled */
void initGraph(Graph *gra, int V, int E, int *degree);

/** frees all allocated space */
void deleteGraph(Graph *gra);

/** prints the adjacency lists and |V| and |E| */
void printGraph(Graph *graph);

#endif
