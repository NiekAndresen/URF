/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

/* Struct defintions
exact compositions are not part of the interface/not important for the user */
#include "./URFcode/URF_graph_Struct.h"
#include "./URFcode/URF_data_Struct.h"

/** An atom is represented by its index in the molecule (0 to |V|-1).
This typedef seperates atoms from other integers. */
typedef unsigned int URFAtom;
/** A bond is represented by an array of size two containing the atoms that it
connects. */
typedef URFAtom URFBond[2];

/*============================================================================*/
/** build graph data structure that is used for the URF calculation.
The vertices are numbered from 0 to |V|-1. Call the following functions:

 - initNewGraph(int V) to initialize a new graph with V vertices
 (returns URF_graph pointer: includes have provided "typedef
 struct{...} URF_graph" so one can declare a URF_graph *)
 
 - addUEdge(URF_graph *, int from, int to) to add a new (undirected) edge from
 the vertex with index "from" to the vertex with index "to". It is NO problem
 to add an edge twice - even with different order of vertices (will be ignored).
 
now URF_calculate can be called on it*/

/** initializes a new Graph that edges can be added to */
URF_graph *URF_initNewGraph(unsigned int V);

/** adds an undirected edge to the graph. */
void URF_addUEdge(URF_graph *, URFAtom from, URFAtom to);
/*============================================================================*/

/** Calculates the URF-structure of the given graph and returns it (includes
have provided "typedef struct{...} URF_data" so one can declare a URF_data *).
The graph has to be a correct undirected graph.*/
URF_data *URF_calculate(URF_graph *);

/** Deletes URF_data from memory including the graph.*/
void URF_deleteData(URF_data *);

/** Returns the number of URFs. */
unsigned int URF_giveNumber(URF_data *);

/** Returns the weight of each cycle in the URF identified by its index */
unsigned int URF_giveWeight(URF_data *, int index);

/** Returns the number of URFs that contain the given atom. */
unsigned int URF_numContaining(URF_data *, URFAtom atom);

/** Returns the number of URFs that contain the bond consisting of the two given
 atoms */
unsigned int URF_numContainingBond(URF_data *, URFAtom atom1, URFAtom atom2);

/*============================================================================*/
/** Gives the atoms of a URF identified with its index in an array of URFatoms.
 @return the number of atoms in the URF
 @param index the index of the URF
 @param ptr pointer that points to the result array (declare 'URFAtom *' and
give address as parameter) 
 @note Result has do be deallocated using free(*ptr).
 @brief Gives the atoms of a URF. */
unsigned int URF_giveAtoms(URF_data *, unsigned int index, URFAtom **ptr);

/** Gives the bonds of a URF identified with its index.
 @return the number of bonds in this URF
 @param index the index of the URF
 @param ptr pointer that points to the result array (declare 'URFAtom **' and
give address as parameter)
 @note Result has to be deallocated using 'free(*ptr)'
 Gives an array of bonds where a bond is represented by two URFAtoms that it
 connects. */
unsigned int URF_giveBonds(URF_data *, unsigned int index, URFBond **ptr);

/** Gives all URFs containing the atom.
 @return the number of URFs containing the atom
 @param ptr pointer that points to the result array of integers containing all
indices of URFs containing the atom. (declare 'int *' and give address as
parameter)
 @param atom the URFatom
 @note The array ptr has to be to be deallocated with 'free(*ptr)' */
unsigned int URF_listURFsWithAtom(URF_data *, unsigned int **ptr, URFAtom atom);

/** Gives all URFs containing the bond.
 @return the number of URFs containing the bond
 @param ptr pointer that points to the result array of integers containing all
 indices of URFs containing the bond. (declare  'int *' and give address as
 parameter)
 @param atom1 the first URFAtom of the bond
 @param atom2 the the second URFatom of the bond
 @note The array ptr has to be to be deallocated with 'free(*ptr)' */
unsigned int URF_listURFsWithBond(URF_data *, unsigned int **ptr, URFAtom atom1,
                              URFAtom atom2);

/*============================================================================*/
/** This structure holds a cycle (or ring) in the molecule.
It is essentially an array of bonds which are represented by pairs of atoms.
The length of this array is given by 'weight'. */
typedef struct{
    URFBond *bonds;
    unsigned int weight;
} URFCycle;

/** Gives all cycles of the URF with the given index.
 @return the number of cycles found
 @param ptr pointer that points to the result array of cycles
(declare 'URFCycle *' and give address as parameter)
 @param index index of the URF
 @note ptr has to be deallocated using 'URF_deleteCycles(*ptr)'*/
unsigned int URF_giveCycles(URF_data *, URFCycle **ptr, unsigned int index);

/** Returns a set of cycles that forms a Minimal Cycle Basis of the graph.
 @return the number of cycles returned (|E|-|V|+1)
 @param ptr pointer that points to the result array
(declare 'URFCycle *' and give address as parameter)
 @note Result has to be deallocated using URF_deleteCycles(*ptr, number).
 @note Does not return a correct basis for an unconnected graph.
 The result is an array of cycles.*/
unsigned int URF_findBasis(URF_data *, URFCycle **ptr);

/** @brief Gives a list of relevant cycle prototypes (one for each RCF).
 @return the number of prototypes
 @param ptr pointer to the result array
(declare 'URFCycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using URF_deleteCycles(*ptr, number). */
unsigned int URF_giveRCPrototypes(URF_data *, URFCycle **ptr);

/** @brief Gives a list of all relevant cycles
 @return the number of cycles
 @param ptr pointer to the result array
(declare 'URFCycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using URF_deleteCycles(*ptr, number). */
unsigned int URF_giveRCycles(URF_data *, URFCycle **ptr);

/** Deallocates the structure given by 'URF_giveCycles()', 'findBasis()',
'URF_giveRCPrototypes()' and 'URF_giveRCycles()' if called on its result and return
value (the number of cycles) */
void URF_deleteCycles(URFCycle *cycles, unsigned int number);

/*============================================================================*/

/** Translates the results of 'URF_giveCycles()', 'findBasis()',
'URF_giveRCPrototypes()' and 'URF_giveRCycles()' (arrays of URFcylces) into an array
of cycles represented by arrays of {0,1}^|E|. Such an array has a 1 at position
i if edge i is part of the cycle or 0 otherwise. An edge is identified by the
position at which it was added to the graph starting at 0.
 @param array The resulting structure of the functions named above
 @param number The return value of the functions named above
(the number of cycles given)
 @param ptr Pointer to the result array
(declare 'char **' and give address as parameter)
 @return The number of cycles given (same as the parameter number)
 @brief Changes the representation of a cycle in an array of cycles.
 @note The initial structure still exists afterwards and still has to be
deleted. */
unsigned int URF_translateCycArray(URF_data *, URFCycle *array,
                                 unsigned int number, char ***ptr);

/** Deallocates the structure given by 'URF_translateCycArray()' if called on
its result and return value (the number of cycles). */
void URF_deleteEdgeIdxArray(char **cycles, unsigned int number);

/** Gives the edges of the graph.
 Having the edges of the graph enumerated by this functions helps understand the
 bitset representation given by "URF_translateCycArray()".
 @return the number of edges in the graph
 @param ptr pointer that points to the result array (declare 'URFBond *' and
give address as parameter)
 @note Result has to be deallocated using 'free(*ptr)'
 Gives an array of bonds where a bond is represented by two URFAtoms that it
 connects. */
unsigned int URF_giveEdgeArray(URF_data *, URFBond **ptr);

#endif
