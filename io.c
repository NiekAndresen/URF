/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "graphURF.h"

/*for debugging purposes*/
void printmat(char **mat, int maxRow, int maxCol)
{
    int i, j;
    for(i=0; i<=maxRow; ++i)
    {
        for(j=0; j<=maxCol; ++j)
        {
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
}

GraphURF *readInList(char *path)
{
    GraphURF *gra;
    int temp;
    int V;
    char cha;
    int currLine=0;
    FILE *fp;
    fp = fopen(path, "r");
    
    cha = fgetc(fp);
    while(cha != EOF) //first run through the file to find |V|
    {
        if(cha == '\n')
        {
            ++currLine;
        }
        cha = fgetc(fp);
    }
    V = currLine;
    gra = initNewGraph(V);
    
    rewind(fp);
    currLine = 0;
    cha=fgetc(fp);
    while(cha != EOF) //second run through the file to fill the adjacency lists
    {
        if(cha == ' ')
        {
            fscanf(fp, "%d", &temp);
            addUEdge(gra, currLine, temp-1);
        }
        else if(cha == '\n')
        {
            ++currLine;
        }
        cha = fgetc(fp);
    }
    fclose(fp);
    
    enumerateEdges(gra);
    return gra;
}

void usage()
{
    fprintf(stderr, "The input file must contain adjacency lists of an \n");
    fprintf(stderr, "undirected Graph.\n");
    fprintf(stderr, "one list in one line with a space before each new ");
    fprintf(stderr, "adjacent vertex.\n");
    fprintf(stderr, "ex: vertex 1 is adjacent to vertices 7 and 16, vertex 2 is adjacent \n");
    fprintf(stderr, "to only vertex 4:\n");
    fprintf(stderr, "1 7 16\n");
    fprintf(stderr, "2 4\n");
    exit(EXIT_FAILURE);
}
