/* Niek Andresen 2015 - Bachelor Thesis */

/** graph data structure that is used for the URF calculation. The vertices have to be numbered from 0 to |V|-1. Call the following functions:

 - initNewGraph(int V) to initialize a new graph with V vertices (returns GraphURF pointer)
 
 - addUEdge(GraphURF *, int from, int to) to add a new (undirected) edge from the vertex with index "from" to the vertex with index "to". It is NO problem to add an edge twice - even with different order of vertices (will be ignored).
 
now calculateURFs (from URF.h) can be called on it 

 - deleteGraph to free all allocated space */

#ifndef GRAPHURF_H
#define GRAPHURF_H

#include "graphURFStruct.h"

/** initializes a new Graph that edges can be added to (allocates space for it) */
GraphURF *initNewGraph(int V);

/** adds an undirected edge to the graph. */
void addUEdge(GraphURF *, int from, int to);

/** frees all allocated space. */
void deleteGraph(GraphURF *gra);

/**====== These are not needed for URF.h user: =====*/

/** Fills the 'edges' array which contains all edges as pairs of vertices (smaller vertex index first). (Should be called on complete graph.) */
void enumerateEdges(GraphURF *gra);

/** adds a directed edge to the graph. */
void addEdge(GraphURF *, int from, int to);

/** returns 1 if the vertex i is adjacent to j in the graph, 0 otherwise. */
int isAdj(GraphURF *, int i, int j);

/** prints the adjacency lists, |V| and |E| and the edges */
void printGraph(GraphURF *graph);

/** Checks if the graph is connected. Returns 1 if it is, 0 otherwise. */
char checkGraphCorrect(GraphURF *gra);

/*for undirected graphs the degree is the OUT-degree only.*/
#endif
