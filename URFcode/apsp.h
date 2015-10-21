/** Niek Andresen 2015 - Bachelor Thesis */

#ifndef APSP_H
#define APSP_H

#include "graphURF.h"

#include "apspStruct.h"

/** Solves the "All Pairs Shortest Paths" on the Graph and returns the result. Allocates space for dPaths, but does not initialize it. */
sPathInfo *AllPairsShortestPaths(GraphURF *gra);

/** Deletes everything that 'AllPairsShortestPaths' has created. Needs the number of vertices to delete dPaths. */
void deleteAPSP(sPathInfo *info, int);

#endif
