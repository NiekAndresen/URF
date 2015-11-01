/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef APSPDATASTRUCT
#define APSPDATASTRUCT

#include "URFgraphStruct.h"

typedef struct{
  int **pred; /*predecessor matrix*/
  int **dist; /*distance matrix*/
  char **reachable; /*reachable vertices for each vertex via shortest paths
     only using vertices preceding in order pi (1 or 0/reachable or not)*/
  URF_graph **dPaths; /*for each vertex: a subgraph of the original graph that
     stores the shortest paths back to the vertex (in Vismara: U_r).Useful
     to find paths/edges of a RCF.*/
}sPathInfo;

#endif
