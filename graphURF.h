/* Niek Andresen 2015 - Bachelor Thesis */

/** graph data structure that is used for the URF calculation. The vertices have to be numbered from 0 to |V|-1. Call the following functions:

 - initNewGraph(int V) to initialize a new graph with V vertices (returns GraphURF pointer)
 
 - addUEdge(GraphURF *, int from, int to) to add a new (undirected) edge from the vertex with index "from" to the vertex with index "to". It is NO problem to add an edge twice - even with different order of vertices (will be ignored).
 
after all edges have been added:
 - enumerateEdges(GraphURF *) to complete the structure
 
now calculateURFs (from URF.h) can be called on it 

 - deleteGraph to free all allocated space */

#ifndef GRAPHURF_H
#define GRAPHURF_H

typedef struct{
    int V, E; /*number of vertices/edges*/
    int *degree; /*array storing how many vertices are adjacent to each vertex*/
    int **adjList; /*the vertices are stored with their index (0 to |V|-1)*/
    char edgesEnumerated; /*flag that stores if edges are enumerated (if so: 'y' or any character otherwise)*/
    int **edges; /*array containing pairs of vertices as edges*/
    int *startIdxEdges; /*to navigate the edges array quicker: array containing the index at which the edges from a vertex start in the edges array*/
} GraphURF;

/** initializes a new Graph that edges can be added to (allocates space for it) */
GraphURF *initNewGraph(int V);

/** adds an undirected edge to the graph. */
void addUEdge(GraphURF *, int from, int to);

/** Fills the 'edges' array which contains all edges as pairs of vertices (smaller vertex index first). (Should be called on complete graph.) */
void enumerateEdges(GraphURF *gra);

/** frees all allocated space. */
void deleteGraph(GraphURF *gra);


/**====== These are not needed for URF.h user: =====*/
/** adds a directed edge to the graph. */
void addEdge(GraphURF *, int from, int to);

/** returns 1 if the vertex i is adjacent to j in the graph, 0 otherwise. */
int isAdj(GraphURF *, int i, int j);

/** prints the adjacency lists, |V| and |E| and the edges */
void printGraph(GraphURF *graph);


#endif
