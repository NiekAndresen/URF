/* Niek Andresen 2015 - Bachelor Thesis */

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

/** initializes a new Graph that edges can be added to */
GraphURF *initNewGraph(int V);

/** adds a directed edge to the graph. */
void addEdge(GraphURF *, int from, int to);

/** adds an undirected edge to the graph. */
void addUEdge(GraphURF *, int from, int to);

/** Fills the 'edges' array which contains all edges as pairs of vertices (smaller vertex index first). (Should be called on complete graph.) */
void enumerateEdges(GraphURF *gra);

/** returns 1 if the vertex i is adjacent to j in the graph, 0 otherwise. */
int isAdj(GraphURF *, int i, int j);

/** frees all allocated space. */
void deleteGraph(GraphURF *gra);

/** prints the adjacency lists, |V| and |E| and the edges */
void printGraph(GraphURF *graph);

/** initializes a new graph with the values |V|, |E| and the array degree, then
    allocates enough space for 'adjList' and 'edges', so that they can be filled.
    Touches and allocates 'edges' and 'startIdxEdges' only if the flag 'enumerateEdges' has been set to 'y'. */
/* currently only used in io.c. Could be replaced there by the new init (see above), not really needed in the heaeder file anymore. */
void initGraph(GraphURF *gra, int V, int E, int *degree, char enumerateEdges);

#endif
