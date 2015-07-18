/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef IO_H
#define IO_H

#include "graphURF.h"

/** reads the adjecency lists in the file path into the Graph.
The vertices are saved with their index (0 - |V|-1) */
void readInList(GraphURF *gra, char *path);

/** exits the program and writes instructions to stderr */
void usage();

#endif
