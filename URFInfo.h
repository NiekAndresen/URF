/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFINFO_H
#define URFINFO_H

#include "graphURF.h"
#include "CycleFamsURF.h"
#include "apsp.h"

typedef struct{
    int nofWeights; /*number of different weights occuring*/
    int *nofProtos; /*number of prototypes for each weight*/
    int ***URFs; /*array of 2D-matrices. For each weight that a CF can have there is a matrix that stores 1 at position [i,j] if RCFs i and j (of this particular weight) are URF-related and 0 otherwise.*/
} URFinfo;

/** deletes the URFinfo */
void deleteURFInfo(URFinfo *);

/** Uses Gaussian elimination to mark potentially URF-related CFs and checks the previously marked CFs for URF-relation. Fills the URFinfo. */
URFinfo *checkURFRelation(cfURF *, GraphURF *, sPathInfo *);

#endif
