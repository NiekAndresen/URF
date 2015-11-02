/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFDATASTRUCT
#define URFDATASTRUCT

#include "apspStruct.h"
#include "CycleFamsURFStruct.h"
#include "URFinfoStruct.h"

/** struct that contains all data that was accumulated during the calculation of
the URF-structure. The URF.h user does not need to change or read anything in
this structure other than by using the functions provided in "URF.h". */
typedef struct{
  URF_graph *graph;
  unsigned int nofURFs; /*number of URFs*/
  cfURF *CFs; /*the cycle families (found by Vismara's algorithm)*/
  URFinfo *urfInfo;/*stores which RCF are URF related and belong to the same URF*/
  sPathInfo *spi; /*shortest paths info*/
} URF_data;

#endif
