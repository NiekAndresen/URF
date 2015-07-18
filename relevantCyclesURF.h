#ifndef RELEVANTCYCLESURF_H
#define RELEVANTCYCLESURF_H

#include "apsp.h"

/** contains the relevant cycles */
typedef struct{
    
} rcURF;

/** uses Vismara's algorithm to find relevant cycles represented by relevant cycle families */
rcURF *findRelCycles(GraphURF *, sPathInfo *);

/** frees all the memory used by the relevant cycles */
void deleteRelCycles(rcURF *);

#endif

