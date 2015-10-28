/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFHANDLER_H
#define URFHANDLER_H

/** writes into the position i of the array of |V| integers if the vertex i is
contained on any shortest path from vertex a to vertex b.
(1 if yes, 0 otherwise)*/
void URF_giveVertices(int a, int b, char *array, URF_graph *, sPathInfo *);

/** writes into the position i of the array of |E| integers if the edge i is
contained on any shortest path from vertex a to vertex b.
(1 if yes, 0 otherwise)*/
void URF_giveEdges(int a, int b, char *array, URF_graph *, sPathInfo *);

/** finds the index of the edge [from,to] in the graph. 'from' and 'to' can be
URF_swapped for the same result. */
int URF_edgeIdx(int from, int to, URF_graph *gra);

/** finds all shortest paths (as elements of {0,1}^n) between vertices r and p
and writes them into the array 'paths' */
void URF_getPaths(int r, int p, char ***paths, int alloced, char mode,
                  URF_graph *, sPathInfo *);

/** combines all the paths out of 'paths1' with all out of 'paths2' to form all
possible cycles and writes them into the result array as elements of {0,1}^n.
Afterwards all allocated space in 'paths1' and 'paths2' is deallocated. */
int URF_combinePaths(char ***paths1, char ***paths2, int p, int q, int x,
                     char ***result, int currIdx, int alloced, char mode,
                     URF_graph *);

#endif
