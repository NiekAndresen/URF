#ifndef URF2_H
#define URF2_H

#include "graph.h"

/** struct that contains all data that was accumulated during the calculation of the URF-structure */
typedef struct{
    //number of URFs
    //URFs (RCFs: (r,p,(x,)q+list of all edges); URF-number)
    //basis info
    //shortest paths info (needed?)
} urfdata;

/** Calculates the URF-structure of the given graph and returns it. */
urfdata *calculateURFs(Graph *);

/** Returns the number of URFs. */
int numberOfURFs(urfdata *);

/** Writes the URFs into a file (subgraphs, sets of edges or sets of vertices) */
void writeIntoFile(urfdata *);

/** Writes a specific URF into a file (mode 'a': atoms, 'b': bonds, 'c': cycles) */
void giveURF(urfdata *, int index, char mode);

/** Writes a MCB into a file */
void findBasis(Graph *);

/** Wirtes a RCB into a file */
void findRCB(Graph *);


#endif
