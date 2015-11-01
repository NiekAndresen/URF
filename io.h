/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef IO_H
#define IO_H

#include "./URFcode/URFgraph.h"

/** reads the adjecency lists in the file path into the Graph.
The vertices are saved with their index (0 - |V|-1).
File must not contain empty lines in between lines with adjacency lists. */
URF_graph *readInList(char *path);

/** prints a 2D char matrix to stdout. */
void printmat(char **, int, int);

#endif
