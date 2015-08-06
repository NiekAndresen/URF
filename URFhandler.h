/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFHANDLER_H
#define URFHANDLER_H

/** writes into the position i of the array of |V| integers if the vertex i is contained on any shortest path from vertex a to vertex b. (1 if yes, 0 otherwise)*/
void giveVertices(int a, int b, char *array, GraphURF *, sPathInfo *);

/** writes into the position i of the array of |E| integers if the edge i is contained on any shortest path from vertex a to vertex b. (1 if yes, 0 otherwise)*/
void giveEdges(int a, int b, char *array, GraphURF *, sPathInfo *);

/** finds the index of the edge [from,to] in the graph. 'from' and 'to' can be swapped for the same result. */
int edgeIdx(int from, int to, GraphURF *gra);

#endif
