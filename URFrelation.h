/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef GAUSSELIMURF_H
#define GAUSSELIMURF_H

#include "URFInfo.h"
#include "apsp.h"

/** Finds potentially URFs-related RCFs with the help of Gaussian Elimination, checks if the relation in fulfilled and stores the result in the URFinfo. */
void findRelations(rcURF *, GraphURF *, URFinfo *, sPathInfo *);

#endif

