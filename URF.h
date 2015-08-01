/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

#include "graphURF.h"
#include "apsp.h"
#include "relevantCyclesURF.h"
#include "URFInfo.h"

/** struct that contains all data that was accumulated during the calculation of the URF-structure */
typedef struct{
    GraphURF *graph;
    int nofURFs;//number of URFs
    rcURF *RCFs;
    URFinfo *urfInfo; //stores which RCFs are URF-related
    //basis info
    sPathInfo *spi;//shortest paths info (needed?)
}urfdata;

/** Calculates the URF-structure of the given graph and returns it. */
urfdata *calculateURFs(GraphURF *);

/** Deletes urfdata from memory. Does not delete the graph.*/
void deleteURFdata(urfdata *);

///** Returns the number of URFs. */
//int numberOfURFs(urfdata *);

//get bonds, cycles, atoms
///** Writes a specific URF (mode 'a': atoms, 'b': bonds, 'c': cycles) */
//void giveURF(urfdata *, int index, char mode);

//for an atom/bond, get all urfs (return anzahl, nummern)

///** Writes a MCB */
//void findBasis(GraphURF *);

///** gives rcps */
//void findRCp(urfdata *);

///** gives rcs */
//void findRCc(urfdata *);


#endif
