/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef GRAPHURFSTRUCT
#define GRAPHURFSTRUCT

typedef struct{
    int V, E; /*number of vertices/edges*/
    int *degree; /*array storing how many vertices are adjacent to each vertex*/
    int **adjList; /*the vertices are stored with their index (0 to |V|-1)*/
    char edgesEnumerated; /*flag that stores if edges are enumerated (if so: 'y' or any character otherwise)*/
    int **edges; /*array containing pairs of vertices as edges*/
    int *startIdxEdges; /*to navigate the edges array quicker: array containing the index at which the edges from a vertex start in the edges array*/
} GraphURF;

#endif
