/** Niek Andresen 2015 - Bachelor Thesis */

#ifndef APSP_H
#define APSP_H

#include "graphURF.h"

typedef struct{
    int **pred; /*predecessor matrix*/
    int **dist; /*distance matrix*/
    char **reachable; /*reachable vertices for each vertex via shortest paths only using vertices preceding in order pi (1 or 0/reachable or not)*/
    GraphURF **dPaths; /*for each vertex: a subgraph of the original graph that stores the shortest paths back to the vertex (in Vismara: U_r).Useful to find paths/edges of a RCF.*/
}sPathInfo;

/** Solves the "All Pairs Shortest Paths" on the Graph and returns the result. Allocates space for dPaths, but does not initialize it. */
sPathInfo *AllPairsShortestPaths(GraphURF *gra);

/** Deletes everything that 'AllPairsShortestPaths' has created. Needs the number of vertices to delete dPaths. */
void deleteAPSP(sPathInfo *info, int);

#endif
