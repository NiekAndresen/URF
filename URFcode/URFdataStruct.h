/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFDATASTRUCT
#define URFDATASTRUCT

#include "apspStruct.h"
#include "CycleFamsURFStruct.h"
#include "URFInfoStruct.h"

/** struct that contains all data that was accumulated during the calculation of the URF-structure.
The URF.h user does not need to change or read anything in this structure other than by using the functions provided in "URF.h". */
/*TODO something to write: This could contain a flag storing if the URFs have been calculated before for this graph. Every function called would first check if this flag is set and call 'calculateURFs()' if it is not. This would make the 'calculateURFs()' call unnecessary. I won't do this because it would require the user to initialize his urfdata ** to NULL before calling the function for the first time (or get "used uninitialized.."). Also he would have to have a urfdata ** instead of a urfdata * which requires dereferencing one more time.*/
typedef struct{
    URF_graph *graph;
    unsigned int nofURFs; /*number of URFs*/
    cfURF *CFs; /*the cycle families (found by Vismara's algorithm)*/
    URFinfo *urfInfo; /*stores which RCF are URF related and belong to the same URF*/
    sPathInfo *spi; /*shortest paths info*/
} urfdata;

#endif
