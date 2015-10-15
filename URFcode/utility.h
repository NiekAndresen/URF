/** Niek Andresen 2015 - Bachelor Thesis */

#ifndef UTILITY_H
#define UTILITY_H

int **alloc2DIntArray(int n, int m);
unsigned int **alloc2DUIntArray(int n, int m);
char **alloc2DCharArray(int n, int m);
void delete2DArray(void **arr);
void print2DIntArray(int **arr, int n, int m);
void print2DCharArray(char **arr, int n, int m);

/** writes instructions to stderr */
void usage();

#endif
