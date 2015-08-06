/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFINFO_H
#define URFINFO_H

#include "graphURF.h"
#include "CycleFamsURF.h"
#include "apsp.h"

typedef struct{
    int nofWeights; /*number of different weights of cycle families occuring*/
    int *nofProtos; /*array storing the number of prototypes for each weight*/
    char ***URFrel; /*array of 2D-matrices. For each weight that a CF can have there is a matrix that stores 1 at position [i,j] if RCFs i and j (of this particular weight) are URF-related and 0 otherwise.*/
    int nofURFs; /*number of URFs*/
    cfam ***URFs; /*array of tuples of RCFs that belong to the same URF*/
    int *nofCFsPerURF; /*array that stores the number of RCFs that belong to each URF*/
} URFinfo;

/** deletes the URFinfo */
void deleteURFInfo(URFinfo *);

/** Uses Gaussian elimination to mark potentially URF-related CFs and checks the previously marked CFs for URF-relation. Fills the URFinfo. */
URFinfo *checkURFRelation(cfURF *, GraphURF *, sPathInfo *);

#endif
