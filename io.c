/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "graphURF.h"

/*for debugging purposes*/
void printmat(char **mat, int maxRow, int maxCol)
{
    int i,j;
    for(i=0; i<=maxRow; ++i)
    {
        for(j=0; j<=maxCol; ++j)
        {
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
}

void readInList(GraphURF *gra, char *path)
{
    int i, temp;
    int V, E=0;
    int *degree;
    char cha;
    int currLine=0, currNumOfLines=2, currPos=0;
    FILE *fp;
    fp = fopen(path, "r");
    
    degree = malloc(currNumOfLines * sizeof(*degree));
    for(i=0; i<currNumOfLines; ++i)
    {
        degree[i] = 0;
    }
    cha = fgetc(fp);
    while(cha != EOF) //first run through the file to find |V|, |E| and degree
    {
        if(cha == '\n')
        {
            cha = fgetc(fp);
            ++currLine;
            if(currLine >= currNumOfLines) //additional space for lines needed
            {
                currNumOfLines *= 2;
                degree = realloc(degree, currNumOfLines * sizeof(*degree));
                for(i=currLine; i<currNumOfLines; ++i)
                {
                    degree[i] = 0;
                }
            }
        }
        if(cha == ' ')
        {
            cha = fgetc(fp);
            if(isdigit(cha)) //space followed by a number (1-9)
            {
                ++degree[currLine];
                ++E;
            }
        }
        cha = fgetc(fp);
    }
    V = currLine;
    E = E/2; //edges appear twice in adj-lists
    initGraph(gra, V, E, degree, 'y');
    
    rewind(fp);
    currLine = 0;
    cha=fgetc(fp);
    while(cha != EOF) //second run through the file to fill the adjacency lists
    {
        if(cha == '\n')
        {
            ++currLine;
            currPos = 0;
        }
        if(cha == ' ')
        {
            fscanf(fp, "%d", &temp);
            gra->adjList[currLine][currPos] = temp - 1; /*shift in index*/
            ++currPos;
        }
        cha = fgetc(fp);
    }
    
    fclose(fp);
    
    enumerateEdges(gra);
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
