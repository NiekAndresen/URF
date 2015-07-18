/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef GRAPHURF_H
#define GRAPHURF_H

typedef struct{
    int V, E; /*number of vertices/edges*/
    int *degree; /*array storing how many vertices are adjacent to each vertex*/
    int **adjList; /*the vertices are stored with their index (0 to |V|-1)*/
    int **edges; /*array containing pairs of vertices as edges*/
} GraphURF;

/** initializes a new graph with the values |V|, |E| and the array degree, then
    allocates enough space for 'adjList' and 'edges', so that they can be filled */
void initGraph(GraphURF *gra, int V, int E, int *degree);

/** frees all allocated space */
void deleteGraph(GraphURF *gra);

/** prints the adjacency lists, |V| and |E| and the edges */
void printGraph(GraphURF *graph);

#endif
