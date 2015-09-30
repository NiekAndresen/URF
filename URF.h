/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

#include "./URFcode/graphURF.h"
#include "./URFcode/URFdataStruct.h"

/*TODO write something about calling these functions on a graph without cycles: advantage of returned number of cycles etc: can be 0; still possible to call delete functions; ...*/

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

/** Gives the atoms of a URF identified with its index in an array of indices of atoms.
 @return the number of atoms in the URF
 @param index the index of the URF
 @param ptr pointer that points to the result array (declare 'int *' and give address as parameter) 
 @note Result has do be deallocated using free(*ptr).
 @brief Gives the atoms of a URF. */
int giveURFAtoms(urfdata *, int index, int **ptr);

/** Gives the bonds of a URF identified with its index.
 @return the number of bonds in this URF
 @param index the index of the URF
 @param ptr pointer that points to the result array (declare 'int **' and give address as parameter)
 @note Result has to be deallocated using 'deleteBondArr(*ptr)'
 Gives an array of bonds where a bond is represented by two indices of vertices that it connects. */
int giveURFBonds(urfdata *, int index, int ***ptr);

/** Deallocates the structure returned by 'giveURFBonds()' if called on its return value. */
void deleteBondArr(int **);

/** Gives all cycles of the URF with the given index.
@return the number of cycles found
@note ptr has to be deallocated using 'deleteCycles(*ptr)'
@note A cycle is represented by an array of bonds which are arrays of length two of integers storing the indices of the atoms involved in the bond.
@note For iteration over a cycle, its array is ended with a NULL pointer.
@param ptr pointer that points to the result array of cycles (declare 'int ***' and give address as parameter)
@param index index of the URF*/
/*TODO write an example for iteration over a cycle/an array of cycles somewhere*/
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

/** Returns a set of cycles that forms a Minimal Cycle Basis of the graph.
 @return the number of cycles returned (|E|-|V|+1)
 @param ptr pointer that points to the result array (declare 'int ***' and give address as parameter)
 The result is an array of cycles. A cycle is represented by an array of bonds. A bond is represented by two integers which are the indices of the atoms involved in the bond. A cycle is terminated by a NULL pointer
 @note Result has to be deallocated using deleteCycles(*ptr, number).
 @note Does not return a correct basis for an undirected graph. */
int findBasis(urfdata *, int ****ptr);

/** @brief Gives a list of relevant cycle prototypes (one for each RCF).
 @return the number of prototypes
 @param ptr pointer to the result array (declare 'int ***' and give address as parameter)
 The result is an array of cycles. A cycle is represented by an array of bonds. A bond is represented by two integers which are the indices of the atoms involved in the bond. A cycle is terminated by a NULL pointer.
 @note Result has to be deallocated using deleteCycles(*ptr, number). */
int giveRCprototypes(urfdata *, int ****ptr);

/** @brief Gives a list of all relevant cycles
 @return the number of cycles
 @param ptr pointer to the result array (declare 'int ***' and give address as parameter)
 The result is an array of cycles. A cycle is represented by an array of bonds. A bond is represented by two integers which are the indices of the atoms involved in the bond. A cycle is terminated by a NULL pointer.
 @note Result has to be deallocated using deleteCycles(*ptr, number). */
int giveRCcycles(urfdata *, int ****ptr);

/** Deallocates the structure returned by 'giveURFCycles()', 'findBasis()', 'giveRCprototypes()' and 'giveRCcycles()' if called on its result and return value (the number of cycles) */
void deleteCycles(int ***cycles, int number);

#endif
