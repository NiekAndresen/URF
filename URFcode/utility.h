/** Niek Andresen 2015 - Bachelor Thesis */

#ifndef UTILITY_H
#define UTILITY_H

int **URF_alloc2DIntArray(int n, int m);
unsigned int **URF_alloc2DUIntArray(int n, int m);
char **URF_alloc2DCharArray(int n, int m);
void URF_delete2DArray(void **arr);
void URF_print2DIntArray(int **arr, int n, int m);
void URF_print2DCharArray(char **arr, int n, int m);

/** writes instructions to stderr */
void URF_warn();

#endif
