/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

#include "./URFcode/apsp.h"
#include "./URFcode/CycleFamsURF.h"
#include "./URFcode/URFInfo.h"
#include "./URFcode/graphURF.h"
#include "./URFcode/URFdataStruct.h"

/*===================================================================================*/
/** build graph data structure that is used for the URF calculation. The vertices are numbered from 0 to |V|-1. Call the following functions:

 - initNewGraph(int V) to initialize a new graph with V vertices (returns GraphURF pointer: includes have provided "typedef struct{...} GraphURF" so one can declare a GraphURF *)
 
 - addUEdge(GraphURF *, int from, int to) to add a new (undirected) edge from the vertex with index "from" to the vertex with index "to". It is NO problem to add an edge twice - even with different order of vertices (will be ignored).
 
now calculateURFs (from URF.h) can be called on it*/
 
/** initializes a new Graph that edges can be added to (allocates space for it) */
GraphURF *initNewURFGraph(int V);

/** adds an undirected edge to the graph. */
void addUEdgeURF(GraphURF *, int from, int to);
/*===================================================================================*/

/** Calculates the URF-structure of the given graph and returns it (includes have provided "typedef struct{...} urfdata" so one can declare a urfdata *). The graph has to be a correct undirected graph.*/
urfdata *calculateURFs(GraphURF *);

/** Deletes urfdata from memory including the graph.*/
void deleteURFdata(urfdata *);

/** Returns the number of URFs. */
int numberOfURFs(urfdata *);

/** Gives the atoms of a URF identified with it's index.
Returns an array of length n (Number of atoms in the molecule) that contains 1 at position i if atom i is part of the URF.
Return value has to be deallocated using free(). */
int *giveURFAtoms(urfdata *, int index);

/** Gives the bonds of a URF identified with it's index.
Returns an array of bonds which ends on a terminating NULL pointer. A bond is represented by two integers which are the indices of the atoms the bond connects.
Return value has to be deallocated using 'deleteArr()'.*/
int **giveURFBonds(urfdata *, int index);

/** Gives all cycles of the URF with the given index.
@return the number of cycles found
@note ptr has to be deallocated using 'deleteCycles(ptr)'
@note A cycle is represented by an array of bonds which are arrays of length two of integers storing the indices of the atoms involved in the bond.
@note For iteration over a cycle, it's array is ended with a NULL pointer.
@param ptr pointer that points to the result array of cycles (declare 'int ***' and give address as parameter)
@param index index of the URF*/
int giveURFCycles(urfdata *, int ****ptr, int index);

/** Gives all URFs containing the atom.
@return the number of URFs containing the atom
@param ptr pointer that points to the result array of integers containing all indices of URFs containing the atom. (declare 'int *' and give address as parameter)
@param atom the index of the atom
@note The array ptr has to be to be deallocated with 'free(*ptr)' */
int listURFsWithAtom(urfdata *, int **ptr, int atom);

/** Gives all URFs containing the bond.
@return the number of URFs containing the bond
@param ptr pointer that points to the result array of integers containing all indices of URFs containing the bond. (declare 'int *' and give address as parameter)
@param a1 the index of one of the atoms adjacent to the bond
@param a2 the index of one of the atoms adjacent to the bond
@note The array ptr has to be to be deallocated with 'free(*ptr)' */
int listURFsWithBond(urfdata *, int **ptr, int a1, int a2);

/** Returns the number of URFs that contain the given atom. */
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

/** Deallocates the structure returned by 'giveURFCycles()' if called on its return value */
/* giveRCcycles() */
void deleteCycles(int ***cycles, int number);

/** Deallocates the structure returned by 'findBasis()' and 'giveRCprototypes()' if called on their return values. */
void deleteArr(char **);

/** Deallocates the structure returned by 'giveURFBonds()' if called on it's return value. */
void deleteBondArr(int **);

#endif
