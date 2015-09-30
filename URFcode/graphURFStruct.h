/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef GRAPHURFSTRUCT
#define GRAPHURFSTRUCT

typedef struct{
    int V, E; /*number of vertices/edges*/
    int *degree; /*array storing how many vertices are adjacent to each vertex*/
    int **adjList; /*the vertices are stored with their index (0 to |V|-1)*/
    int **edges; /*array containing pairs of vertices as edges*/
    int edgesAlloced; /*space alloced for this many edges in 'edges' array*/
} GraphURF;

#endif
