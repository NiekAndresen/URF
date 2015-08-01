/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef RELEVANTCYCLESURF_H
#define RELEVANTCYCLESURF_H

#include "apsp.h"

/** a relevant cycle family */
typedef struct{
    int weight; /*number of edges/vertices in the rcf (length of one cycle)*/
    int r, p, q, x; /*vertices defining a rcf as in Vismara's definition*/
    char *prototype; /*prototype vector {0,1}^m*/
    int mark; /*mark RCF as part of the Basis*/
}rcf;

/** contains the relevant cycles */
typedef struct{
    rcf **fams; /*array of RCFs sorted by weight*/
    int nofFams; /*number of relevant cycle families*/
} rcURF;

/** uses Vismara's algorithm to find relevant cycles represented by relevant cycle families. returns them sorted by weight */
rcURF *findRelCycles(GraphURF *, sPathInfo *);

/** frees all the memory used by the relevant cycles */
void deleteRelCycles(rcURF *);

#endif

