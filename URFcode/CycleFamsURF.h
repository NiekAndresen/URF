/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef CYCLEFAMSURF_H
#define CYCLEFAMSURF_H

#include "apsp.h"
#include "CycleFamsURFStruct.h"

/** uses Vismara's algorithm to find (Relevant) Cycle Families.
Returns them sorted by weight */
cfURF *URF_findCycleFams(URF_graph *, sPathInfo *);

/** frees all the memory used by the Cycle Families */
void URF_deleteCycleFams(cfURF *);

#endif
