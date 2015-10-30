/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef CFQUICKSORT_H
#define CFQUICKSORT_H

#include "CycleFamsURFStruct.h"

/** sorts the RCFs in the array fams by weight. */
void URF_sortbyweight(cfURF *rc);

/* qsort() of the C standard library could be used just as well */

#endif
