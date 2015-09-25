/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef CYCLEFAMSURF_H
#define CYCLEFAMSURF_H

#include "apsp.h"

/** a cycle family */
typedef struct{
    int weight; /*number of edges/vertices in the cf (length of one cycle)*/
    int r, p, q, x; /*vertices defining a cf as in Vismara's definition*/
    char *prototype; /*prototype vector {0,1}^m*/
    int mark; /*mark CF as relevant*/
}cfam;

/** contains the Cycle Families */
typedef struct{
    cfam **fams; /*array of CFs sorted by weight*/
    int nofFams; /*number of cycle families*/
}cfURF;

/** uses Vismara's algorithm to find (Relevant) Cycle Families. returns them sorted by weight */
cfURF *findCycleFams(GraphURF *, sPathInfo *);

/** frees all the memory used by the Cycle Families */
void deleteCycleFams(cfURF *);

#endif
