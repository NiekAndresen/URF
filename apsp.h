/** Niek Andresen 2015 - Bachelor Thesis */

#ifndef APSP_H
#define APSP_H

typedef struct{
    int **pred; //predecessor matrix
    int **dist; //distance matrix
    char **reachable; //reachable vertices for each vertex via shortest paths only using vertices preceding in order pi (1 or 0/reachable or not)
}sPathInfo;

/** Solves the "All Pairs Shortest Paths" on the Graph and returns the result */
sPathInfo *AllPairsShortestPaths(Graph *gra);

/** Deletes everything that 'AllPairsShortestPaths' has created */
void deleteAPSP(sPathInfo *info);

#endif
