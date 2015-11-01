/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URF_H
#define URF_H

/* Struct defintions
exact compositions are not part of the interface/not important for the user */
#include "./URFcode/URFgraphStruct.h"
#include "./URFcode/URFdataStruct.h"

/** An atom is represented by its index in the molecule (0 to |V|-1).
This typedef seperates atoms from other integers. */
typedef unsigned int URF_atom;
/** A bond is represented by an array of size two containing the atoms that it
connects. */
typedef URF_atom URF_bond[2];

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
URF_graph *URF_initNewGraph(unsigned int nof_atoms);

/** adds an undirected edge to the graph. */
void URF_addUEdge(URF_graph *, URF_atom atom1, URF_atom atom2);
/*============================================================================*/

/** Calculates the URF-structure of the given graph and returns it (includes
have provided "typedef struct{...} URF_data" so one can declare a URF_data *).
The graph has to be a correct undirected graph.*/
URF_data *URF_calculate(URF_graph *input_graph);

/** Deletes URF_data from memory including the graph.*/
void URF_deleteData(URF_data *);

/** Returns the number of URFs. */
unsigned int URF_getNumber(URF_data *);

/** Returns the weight of each cycle in the URF identified by its index */
unsigned int URF_getWeight(URF_data *, int URF_id);

/** Returns the number of URFs that contain the given atom. */
unsigned int URF_numContainingAtom(URF_data *, URF_atom atom);

/** Returns the number of URFs that contain the bond consisting of the two given
 atoms */
unsigned int URF_numContainingBond(URF_data *, URF_atom atom1, URF_atom atom2);

/*============================================================================*/
/** Gives the atoms of a URF identified with its index in an array of URF_atoms.
 @return the number of atoms in the URF
 @param URF_id the index of the URF
 @param URF_atom_array_ptr pointer that points to the result array (declare
 'URF_atom *' and give address as parameter) 
 @note Result has do be deallocated using free(*URF_atom_array_ptr).
 @brief Gives the atoms of a URF. */
unsigned int URF_getAtoms(URF_data *, unsigned int URF_id,
                          URF_atom **URF_atom_array_ptr);

/** Gives the bonds of a URF identified with its index.
 @return the number of bonds in this URF
 @param URF_id the index of the URF
 @param URF_bond_array_ptr pointer that points to the result array (declare
 'URF_atom **' and give address as parameter)
 @note Result has to be deallocated using 'free(*URF_bond_array_ptrptr)'
 Gives an array of bonds where a bond is represented by two URF_atoms that it
 connects. */
unsigned int URF_getBonds(URF_data *, unsigned int URF_id,
                          URF_bond **URF_bond_array_ptr);

/** Gives all URFs containing the atom.
 @return the number of URFs containing the atom
 @param URF_ids_ptr pointer that points to the result array of integers
containing all indices of URFs containing the atom. (declare 'int *' and give
address as parameter)
 @param atom the URF_atom
 @note The array URF_ids_ptr has to be to be deallocated with
'free(*URF_ids_ptr)' */
unsigned int URF_listURFsWithAtom(URF_data *, unsigned int **URF_ids_ptr,
                                  URF_atom atom);

/** Gives all URFs containing the bond.
 @return the number of URFs containing the bond
 @param URF_ids_ptr pointer that points to the result array of integers
containing all indices of URFs containing the bond. (declare  'int *' and give
address as parameter)
 @param atom1 the first URF_atom of the bond
 @param atom2 the the second URF_atom of the bond
 @note The array URF_ids_ptr has to be to be deallocated with
'free(*URF_ids_ptr)' */
unsigned int URF_listURFsWithBond(URF_data *, unsigned int **URF_ids_ptr,
                                  URF_atom atom1, URF_atom atom2);

/*============================================================================*/
/** This structure holds a cycle (or ring) in the molecule.
It is essentially an array of bonds which are represented by pairs of atoms.
The length of this array is given by 'weight'. */
typedef struct{
    URF_bond *bonds;
    unsigned int weight;
} URF_cycle;

/** Gives all cycles of the URF with the given index.
 @return the number of cycles found
 @param URF_cycle_array_ptr pointer that points to the result array of cycles
(declare 'URF_cycle *' and give address as parameter)
 @param URF_id index of the URF
 @note URF_cycle_array_ptr has to be deallocated using
'URF_deleteCycles(*URF_cycle_array_ptr)'*/
unsigned int URF_getCycles(URF_data *, URF_cycle **URF_cycle_array_ptr,
                           unsigned int URF_id);

/** Returns a set of cycles that forms a Minimal Cycle Basis of the graph.
 @return the number of cycles returned (|E|-|V|+1)
 @param URF_cycle_array_ptr pointer that points to the result array
(declare 'URF_cycle *' and give address as parameter)
 @note Result has to be deallocated using
URF_deleteCycles(*URF_cycle_array_ptr, number).
 @note Does not return a correct basis for an unconnected graph.
 The result is an array of cycles.*/
unsigned int URF_getBasis(URF_data *, URF_cycle **URF_cycle_array_ptr);

/** @brief Gives a list of relevant cycle prototypes (one for each RCF).
 @return the number of prototypes
 @param URF_cycle_array_ptr pointer to the result array
(declare 'URF_cycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using
URF_deleteCycles(*URF_cycle_array_ptr, number). */
unsigned int URF_getRCPrototypes(URF_data *, URF_cycle **URF_cycle_array_ptr);

/** @brief Gives a list of all relevant cycles
 @return the number of cycles
 @param URF_cycle_array_ptr pointer to the result array
(declare 'URF_cycle *' and give address as parameter)
 The result is an array of cycles.
 @note Result has to be deallocated using
URF_deleteCycles(*URF_cycle_array_ptr, number). */
unsigned int URF_getRCycles(URF_data *, URF_cycle **URF_cycle_array_ptr);

/** Deallocates the structure given by 'URF_getCycles()', 'getBasis()',
'URF_getRCPrototypes()' and 'URF_getRCycles()' if called on its result and
return value (the number of cycles) */
void URF_deleteCycles(URF_cycle *cycles, unsigned int number);

/*============================================================================*/

/** Translates the results of 'URF_getCycles()', 'getBasis()',
'URF_getRCPrototypes()' and 'URF_getRCycles()' (arrays of URFcylces) into an
array of cycles represented by arrays of {0,1}^|E|. Such an array has a 1 at
position i if edge i is part of the cycle or 0 otherwise. An edge is identified
by the position at which it was added to the graph starting at 0.
 @param old_array The resulting structure of the functions named above
 @param number The return value of the functions named above
(the number of cycles given)
 @param URF_cycle_array_ptr Pointer to the result array
(declare 'char **' and give address as parameter)
 @return The number of cycles given (same as the parameter number)
 @brief Changes the representation of a cycle in an array of cycles.
 @note The initial structure still exists afterwards and still has to be
deleted. */
unsigned int URF_translateCycArray(URF_data *, URF_cycle *old_array,
                                   unsigned int number,
                                   char ***URF_cycle_array_ptr);

/** Deallocates the structure given by 'URF_translateCycArray()' if called on
its result and return value (the number of cycles). */
void URF_deleteEdgeIdxArray(char **cycles, unsigned int number);

/** Gives the edges of the graph.
 Having the edges of the graph enumerated by this functions helps understand the
 bitset representation given by "URF_translateCycArray()".
 @return the number of edges in the graph
 @param URF_bond_array_ptr pointer that points to the result array
(declare 'URF_bond *' and give address as parameter)
 @note Result has to be deallocated using 'free(*URF_bond_array_ptr)'
 Gives an array of bonds where a bond is represented by two URF_atoms that it
 connects. */
unsigned int URF_getEdgeArray(URF_data *, URF_bond **URF_bond_array_ptr);

#endif
