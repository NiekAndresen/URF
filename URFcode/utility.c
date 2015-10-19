/** Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>

int **alloc2DIntArray(int n, int m)
{
    int i;
    int **arr = malloc(n * sizeof(*arr));
    *arr = malloc(n*m*sizeof(**arr));
    for(i=1; i<n; ++i)
    {
        arr[i] = *arr + i * m;
    }
    return arr;
}

unsigned int **alloc2DUIntArray(int n, int m)
{
    int i;
    unsigned int **arr = malloc(n * sizeof(*arr));
    *arr = malloc(n*m*sizeof(**arr));
    for(i=1; i<n; ++i)
    {
        arr[i] = *arr + i * m;
    }
    return arr;
}

char **alloc2DCharArray(int n, int m)
{
    int i;
    char **arr = malloc(n * sizeof(*arr));
    *arr = malloc(n*m*sizeof(**arr));
    for(i=1; i<n; ++i)
    {
        arr[i] = *arr + i * m;
    }
    return arr;
}

void delete2DArray(void **arr)
{
    free(*arr);
    free(arr);
}

void print2DIntArray(int **arr, int n, int m)
{
    int i,j;
    for(i=0; i<n; ++i)
    {
        for(j=0; j<m; ++j)
        {
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
}

void print2DCharArray(char **arr, int n, int m)
{
    int i,j;
    for(i=0; i<n; ++i)
    {
        for(j=0; j<m; ++j)
        {
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
}

/* errTypes where the program is exited are for debugging. They don't occur when using only URF.h interface */
void URF_warn(char errType)
{
    if(errType == 'c')
    {
        fprintf(stderr, "WARNING: The given molecule was not connected.\n");
        fprintf(stderr, "Read interface comment in URF.h for usage help.\n");
    }
    else if(errType == 'a')
    {
        fprintf(stderr, "WARNING: In URF finder: Tried to add an edge with atoms not in range.\n");
    }
    else if(errType == 'v')
    {
        fprintf(stderr, "ERROR: Tried to initialize a graph with negative number of vertices.\n");
    }
    else if(errType == 'f')
    {
        fprintf(stderr, "ERROR: Could not open the file specified in argv[1].\n");
        fprintf(stderr, "Exiting program.\n");
        exit(EXIT_FAILURE);
    }
    else if(errType == 'e')
    {
        fprintf(stderr, "ERROR: Could not find a requested edge.\n");
        fprintf(stderr, "Exiting program.\n");
        exit(EXIT_FAILURE);
    }
}
