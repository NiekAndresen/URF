#ifndef URF1_H
#define URF1_H

#include "graph.h"

/** Calculates the URFs of the given Graph and writes the corresponding subgraphs, the sets of edges or the sets of atoms of each URF into a file. */
void calculateURFs(Graph *inputgraph, char mode);

/** Returns the number of URFs that the given Graph contains. */
int numberOfURFs(Graph *);

/** Calculates a Minimal Cycle Basis (MCB) of the given Graph and writes it into a file. */
void findBasis(Graph *);

/* other options: -give all edges/vertices/... of a certain URF 
                  -give all relevant cycles
                  -..*/

#endif
