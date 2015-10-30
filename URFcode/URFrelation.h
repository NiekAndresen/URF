/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef GAUSSELIMURF_H
#define GAUSSELIMURF_H

#include "URFInfoStruct.h"
#include "apspStruct.h"

/** Finds potentially URFs-related CFs with the help of Gaussian Elimination,
checks if the relation in fulfilled and stores the result in the URFinfo. In
this step the non-relevant cycle families found before by Vismara's algorithm
are ignored.*/
void URF_findRelations(cfURF *, URF_graph *, URFinfo *, sPathInfo *);

/** Checks the given matrix (rows x cols) for linear dependency. Returns 1 if
the set of rings is URF_dependent or 0 otherwise. */
int URF_linDep(char **mat, int rows, int cols);

#endif

