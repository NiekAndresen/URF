/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFINFO_H
#define URFINFO_H

#include "graphURF.h"
#include "relevantCyclesURF.h"

typedef struct{
    int nofWeights; /*number of different weights occuring*/
    int *nofProtos; /*number of prototypes for each weight*/
    int ***URFs; /*array of 2D-matrices. For each weight that a RCF can have there is a matrix that stores 1 at position [i,j] if RCFs i and j (of this particular weight) are URF-related and 0 otherwise.*/
} URFinfo;

/** Allocates the array in URFinfo. */
URFinfo *initUrfInfo(rcURF *, GraphURF *);

/** deletes the URFinfo */
void deleteURFInfo(URFinfo *);

/** Uses Gaussian elimination to mark potentially URF-related RCFs and checks the previously marked RCFs for URF-relation. Fills the URFinfo. */
URFinfo *checkURFRelation(rcURF *, GraphURF *);

#endif
