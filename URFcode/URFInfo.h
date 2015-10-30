/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFINFO_H
#define URFINFO_H

#include "URF_graph_Struct.h"
#include "CycleFamsURFStruct.h"
#include "apspStruct.h"
#include "URFInfoStruct.h"

/** deletes the URFinfo */
void URF_deleteURFInfo(URFinfo *);

/** Uses Gaussian elimination to mark potentially URF-related CFs and checks
the previously marked CFs for URF-relation. Fills the URFinfo. */
URFinfo *URF_checkURFRelation(cfURF *, URF_graph *, sPathInfo *);

#endif
