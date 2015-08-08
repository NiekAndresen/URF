/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

#include "graphURF.h"
#include "apsp.h"
#include "CycleFamsURF.h"
#include "URFInfo.h"

/** struct that contains all data that was accumulated during the calculation of the URF-structure */
typedef struct{
    GraphURF *graph;
    int nofURFs;//number of URFs
    cfURF *CFs; //the cycle families (found by Vismara's algorithm)
    URFinfo *urfInfo; //stores which RCFs are URF-related
    //basis info
    sPathInfo *spi;//shortest paths info
}urfdata;

/** Calculates the URF-structure of the given graph and returns it. The graph has to be a correct undirected graph and the edges have to be enumerated with 'enumerateEdges' (see graphURF.h).*/
urfdata *calculateURFs(GraphURF *);

/** Deletes urfdata from memory. Does not delete the graph.*/
void deleteURFdata(urfdata *);

/** Returns the number of URFs. */
int numberOfURFs(urfdata *);

/** Gives a specific URF identified with it's index
 modes:
  - 'a': atoms
    returns an array of integers containing all indices of atoms (vertices of the graph) that are part of the URF
  - 'b': bonds
    returns an array of integers containing all indices of bonds (edges of the graph) that are part of the URF
The arrays returned are ended with INT_MAX on their last position and have to be deallocated with 'free()'.*/
/*alternative: return arrays of {0,1}^n and {0,1}^m*/
int *giveURF(urfdata *, int index, char mode);

/** Gives all cycles of the URF with the given index. Returns an array of cycles. A cycle is represented by an array of {0,1}^n which contains a 1 at position i if the vertex i is part of the cycle or 0 otherwise (n: number of vertices).
The array of cycles is ended with a terminating NULL pointer.
This structure has to be deallocated using 'free()'.*/
char **giveURFCycles(urfdata *, int index);

//for an atom/bond, get all urfs (return anzahl, nummern)

///** Writes a MCB */
//void findBasis(GraphURF *);

///** gives rcps */
//void findRCp(urfdata *);

///** gives rcs */
//void findRCc(urfdata *);


#endif
