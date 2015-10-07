/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

/* Struct defintions - exact compositions are not part of the interface/not important for the user */
#include "./URFcode/graphURFStruct.h"
#include "./URFcode/URFdataStruct.h"

/** An atom is represented by its index in the molecule (0 to |V|-1). This typedef seperates atoms from other integers. */
typedef int URFAtom;
/** A bond is represented by the atoms that it connects. */
typedef URFAtom* URFBond;

/*===================================================================================*/
/** build graph data structure that is used for the URF calculation. The vertices are numbered from 0 to |V|-1. Call the following functions:

 - initNewGraph(int V) to initialize a new graph with V vertices (returns GraphURF pointer: includes have provided "typedef struct{...} GraphURF" so one can declare a GraphURF *)
 
 - addUEdge(GraphURF *, int from, int to) to add a new (undirected) edge from the vertex with index "from" to the vertex with index "to". It is NO problem to add an edge twice - even with different order of vertices (will be ignored).
 
now calculateURFs (from URF.h) can be called on it*/
 
/** initializes a new Graph that edges can be added to (allocates space for it) */
GraphURF *initNewURFGraph(int V);

/** adds an undirected edge to the graph. */
void addUEdgeURF(GraphURF *, URFAtom from, URFAtom to);
/*===================================================================================*/

/** Calculates the URF-structure of the given graph and returns it (includes have provided "typedef struct{...} urfdata" so one can declare a urfdata *). The graph has to be a correct undirected graph.*/
urfdata *calculateURFs(GraphURF *);

/** Deletes urfdata from memory including the graph.*/
void deleteURFdata(urfdata *);

/** Returns the number of URFs. */
int numberOfURFs(urfdata *);

/** Returns the weight of each cycle in the URF identified by its index */
int giveURFWeight(urfdata *, int index);

/** Returns the number of URFs that contain the given atom. */
int numOfURFsContaining(urfdata *, URFAtom atom);

/** Returns the number of URFs that contain the bond consisting of the two given atoms */
int numOfURFsContainingBond(urfdata *, URFAtom atom1, URFAtom atom2);

/*===================================================================================*/
/** Gives the atoms of a URF identified with its index in an array of URFatoms.
 @return the number of atoms in the URF
 @param index the index of the URF
 @param ptr pointer that points to the result array (declare 'URFAtom *' and give address as parameter) 
 @note Result has do be deallocated using free(*ptr).
 @brief Gives the atoms of a URF. */
int giveURFAtoms(urfdata *, int index, URFAtom **ptr);

/** Gives the bonds of a URF identified with its index.
 @return the number of bonds in this URF
 @param index the index of the URF
 @param ptr pointer that points to the result array (declare 'URFAtom **' and give address as parameter)
 @note Result has to be deallocated using 'deleteBondArr(*ptr)'
 Gives an array of bonds where a bond is represented by two URFAtoms that it connects. */
int giveURFBonds(urfdata *, int index, URFBond **ptr);

/** Deallocates the structure returned by 'giveURFBonds()' if called on its return value. */
void deleteBondArr(URFBond *);

/** Gives all URFs containing the atom.
@return the number of URFs containing the atom
@param ptr pointer that points to the result array of integers containing all indices of URFs containing the atom. (declare 'int *' and give address as parameter)
@param atom the URFatom
@note The array ptr has to be to be deallocated with 'free(*ptr)' */
int listURFsWithAtom(urfdata *, int **ptr, URFAtom atom);

/** Gives all URFs containing the bond.
@return the number of URFs containing the bond
@param ptr pointer that points to the result array of integers containing all indices of URFs containing the bond. (declare 'int *' and give address as parameter)
@param atom1 the first URFAtom of the bond
@param atom2 the the second URFatom of the bond
@note The array ptr has to be to be deallocated with 'free(*ptr)' */
int listURFsWithBond(urfdata *, int **ptr, URFAtom atom1, URFAtom atom2);

/*===================================================================================*/
/** This structure holds a cycle (or ring) in the molecule.
It is essentially an array of bonds which are represented by pairs of atoms.
The length of this array is given by 'weight'. */
typedef struct{
    URFAtom **bonds;
    int weight;
} URFCycle;

/** Gives all cycles of the URF with the given index.
@return the number of cycles found
@param ptr pointer that points to the result array of cycles (declare 'URFCycle *' and give address as parameter)
@param index index of the URF
@note ptr has to be deallocated using 'deleteCycles(*ptr)'*/
int giveURFCycles(urfdata *, URFCycle **ptr, int index);

/** Returns a set of cycles that forms a Minimal Cycle Basis of the graph.
 @return the number of cycles returned (|E|-|V|+1)
 @param ptr pointer that points to the result array (declare 'URFCycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using deleteCycles(*ptr, number).
 @note Does not return a correct basis for an undirected graph. */
int findBasis(urfdata *, URFCycle **ptr);

/** @brief Gives a list of relevant cycle prototypes (one for each RCF).
 @return the number of prototypes
 @param ptr pointer to the result array (declare 'URFCycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using deleteCycles(*ptr, number). */
int giveRCprototypes(urfdata *, URFCycle **ptr);

/** @brief Gives a list of all relevant cycles
 @return the number of cycles
 @param ptr pointer to the result array (declare 'URFCycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using deleteCycles(*ptr, number). */
int giveRCcycles(urfdata *, URFCycle **ptr);

/** Deallocates the structure given by 'giveURFCycles()', 'findBasis()', 'giveRCprototypes()' and 'giveRCcycles()' if called on its result and return value (the number of cycles) */
void deleteCycles(URFCycle *cycles, int number);

/*===================================================================================*/

/** Translates the results of 'giveURFCycles()', 'findBasis()', 'giveRCprototypes()' and 'giveRCcycles()' (arrays of URFcylces) into an array of cycles represented by arrays of {0,1}^|E|. Such an array has a 1 at position i if edge i is part of the cycle or 0 otherwise. An edge is identified by the position at which it was added to the graph starting at 0.
 @param array The resulting structure of the functions named above
 @param number The return value of the functions named above (the number of cycles given)
 @param ptr Pointer to the result array (declare 'char **' and give address as parameter)
 @return The number of cycles given (same as the parameter number)
 @brief Changes the representation of a cycle in an array of cycles.
 @note The initial structure still exists afterwards and still has to be deleted. */
int translateCycleArray(urfdata *, URFCycle *array, int number, char ***ptr);

/** Deallocates the structure given by 'translateCycleArray()' if called on its result. */
void deleteEdgeIdxCycles(char **cycles);

#endif
