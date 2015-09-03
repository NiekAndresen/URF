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
    unsigned int nofURFs;//number of URFs
    cfURF *CFs; //the cycle families (found by Vismara's algorithm)
    URFinfo *urfInfo; //stores which RCF are URF related and belong to the same URF
    sPathInfo *spi;//shortest paths info
}urfdata;

/** Calculates the URF-structure of the given graph and returns it. The graph has to be a correct undirected graph.*/
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
The array returned is ended with INT_MAX on its last position and has to be deallocated with 'free()'.*/
/*alternative: return arrays of {0,1}^n and {0,1}^m*/
int *giveURF(urfdata *, int index, char mode);

/** Gives the bonds of a URF identified with it's index.
Returns an array of bonds which ends on a terminating NULL pointer. A bond is represented by two integers which are the indices of the atoms the bond connects.
Return value has to be deallocated using 'deleteArr()'.*/
int **giveURFBonds(urfdata *, int index);

/** Gives all cycles of the URF with the given index. Returns an array of cycles.
modes:
    - 'a': A cycle is represented by an array of {0,1}^n which contains a 1 at position i if the vertex i is part of the cycle or 0 otherwise (n: number of vertices).
    - 'b': A cycle is represented by an array of {0,1}^m which contains a 1 at position i if the edge i is part of the cycle or 0 otherwise (m: number of edges). [problem here: user doesn't know which edge 'the edge with index i' is; the indices of the edges follow an order: go trough the input adjacency lists (ordered by vertex index) and add an edge at next index if it's not been added before]
The array of cycles is ended with a terminating NULL pointer.
This structure has to be deallocated using 'deleteURFCycles(<return value>)'.*/
char **giveURFCycles(urfdata *, int index, char mode);

/** Gives all URFs containing the object, which can be an atom or a bond.
mode:
    - 'a': atom
    - 'b': bond
returns an array of integers containing all indices of URFs containing the object.
The array ends with a terminating INT_MAX on its last position and has to be deallocated with 'free()' */
int *listURFs(urfdata *, int object, char mode);

/** returns the number of URFs that contain the given atom */
int numOfURFsContaining(urfdata *, int atom);

/** returns a set of cycles that forms a MCB of the graph.
A cycle is represented by an array of {0,1}^n with a 1 at position i if vertex i is part of the cycle. The result is an array of these cycles. The result contains |E|-|V|+1 cycles. This does not return a correct basis on an unconnected graph.*/
/*alternatives: return array of vertex indices; give option to call function only on the graph.*/
char **findBasis(urfdata *);

/** Gives a list of relevant cycle prototypes (one for each RCF).
Returns an array of prototypes which are represented by arrays of {0,1}^n containing a 1 at position i if vertex i is part of the cycle or 0 otherwise. The array is ended with a terminating NULL-pointer.*/
char **giveRCprototypes(urfdata *);

/** Gives a list of all relevant cycles.
The return value is an array of cycles. A cycle is represented by an array of {0,1}^n with a 1 at position i if vertex i is part of the cycle or 0 otherwise. The array is ended with a terminating NULL-pointer. */
char **giveRCcycles(urfdata *);

/** Deallocates the structure returned by 'giveURFCycles()' and 'giveRCcycles()' if called on its return value (arrays that are ended with a NULL pointer) */
void deleteCycles(char **);

/** Deallocates the structure returned by 'findBasis()' and 'giveRCprototypes()' if called on their return values. */
void deleteArr(char **);

/** Deallocates the structure returned by 'giveURFBonds()' if called on it's return value. */
void deleteBondArr(int **);

#endif
