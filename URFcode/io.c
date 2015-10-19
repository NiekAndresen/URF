/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "graphURF.h"
#include "../URF.h"
#include "utility.h"

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
    int temp, ignore;
    int V;
    char cha;
    int currLine=0;
    FILE *fp;
    fp = fopen(path, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Could not open file %s\n",path);
        URF_warn('f');
    }
    
    cha = fgetc(fp);
    while(cha != EOF) /*first run through the file to find |V|*/
    {
        if(cha == '\n')
        {
            ++currLine;
        }
        cha = fgetc(fp);
    }
    V = currLine;
    gra = initNewURFGraph(V);
    
    rewind(fp);
    currLine = 0;
    cha=fgetc(fp);
    while(cha != EOF) /*second run through the file to fill the adjacency lists*/
    {
        if(cha == ' ')
        {
            ignore = fscanf(fp, "%d", &temp);
            addUEdgeURF(gra, currLine, temp-1);
            temp = ignore; /*to circumvent a compiler warning of ignoring fscanf return value or "setting but not using" ignore*/
        }
        else if(cha == '\n')
        {
            ++currLine;
        }
        cha = fgetc(fp);
    }
    fclose(fp);

    return gra;
}

