/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef GAUSSELIMURF_H
#define GAUSSELIMURF_H

#include "URFInfo.h"
#include "apsp.h"

/** Finds potentially URFs-related CFs with the help of Gaussian Elimination, checks if the relation in fulfilled and stores the result in the URFinfo. In this step the non-relevant cycle families found before by Vismara's algorithm are ignored.*/
void findRelations(cfURF *, GraphURF *, URFinfo *, sPathInfo *);

#endif

