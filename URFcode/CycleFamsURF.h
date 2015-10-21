/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef CYCLEFAMSURF_H
#define CYCLEFAMSURF_H

#include "apsp.h"
#include "CycleFamsURFStruct.h"

/** uses Vismara's algorithm to find (Relevant) Cycle Families. returns them sorted by weight */
cfURF *findCycleFams(URF_graph *, sPathInfo *);

/** frees all the memory used by the Cycle Families */
void deleteCycleFams(cfURF *);

#endif
