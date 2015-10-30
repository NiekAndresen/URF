/* Niek Andresen 2015 - Bachelor Thesis */

/** graph data structure that is used for the URF calculation.
The vertices have to be numbered from 0 to |V|-1. Call the following functions:

 - initNewGraph(int V) to initialize a new graph with V vertices
    (returns URF_graph pointer)
 
 - addUEdge(URF_graph *, int from, int to) to add a new (undirected) edge from
    the vertex with index "from" to the vertex with index "to". It is NO problem
    to add an edge twice - even with different order of vertices
    (will be ignored).
 
now calculateURFs (from URF.h) can be called on it 

 - URF_deleteGraph to free all allocated space */

#ifndef URF_graph_H
#define URF_graph_H

#include "URF_graph_Struct.h"

/** initializes a new Graph that edges can be added to (allocates space for it) */
URF_graph *URF_initNewGraph_g(int V);

/** adds an undirected edge to the graph. */
void URF_addUEdge_g(URF_graph *, int from, int to);

/** frees all allocated space. */
void URF_deleteGraph(URF_graph *gra);

/*=============================================================================*/
/* This structure can also be used for directed graphs (which is being done in
the calculation of CFs). It that case the degree is the OUT-degree only and the
array edges is not used. */

/** adds a directed edge to the graph. */
void URF_addEdge(URF_graph *, int from, int to);

/** returns 1 if the vertex i is URF_adjacent to j in the graph, 0 otherwise. */
int URF_isAdj(URF_graph *, int i, int j);

/** prints the adjacency lists, |V| and |E| and the edges (edges only if the
    graph is undirected) */
void URF_printGraph(URF_graph *graph);

/** Checks if the graph is connected. Returns 1 if it is, 0 otherwise. */
char URF_checkGraphCorrect(URF_graph *gra);

/** Returns the index of the bond connecting the atoms 'from' and 'to' */
int URF_edgeId(URF_graph *gra, int from, int to);
#endif
