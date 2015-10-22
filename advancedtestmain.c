/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "URF.h"

/** Reads a graph from a file */
URF_graph *readInAdjLists(char *path)
{
    URF_graph *gra;
    int temp;
    int V;
    char cha;
    int currLine=0;
    FILE *fp;
    fp = fopen(path, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "ERROR: Could not open file %s\n",path);
        exit(EXIT_FAILURE);
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
    gra = URF_initNewGraph(V);

    rewind(fp);
    currLine = 0;
    cha=fgetc(fp);
    while(cha != EOF) /*second run through the file to fill the adjacency lists*/
    {
        if(cha == ' ')
        {
            fscanf(fp, "%d", &temp);
            URF_addUEdge(gra, currLine, temp-1);
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

int main(int argc, char **argv)
{
    URF_graph *graph;
    URF_data *URF_data;
    int idx, obIdx, URFcount, count, bondCount;
    URFAtom *atoms;
    URFBond *bondArray;
    URFCycle *cycleArray;
    char **otherCycleArray;

    /* read in the graph */
    if(argc == 1)
    {
        graph = URF_initNewGraph(8);
        URF_addUEdge(graph, 0, 1);
        URF_addUEdge(graph, 0, 2);
        URF_addUEdge(graph, 1, 3);
        URF_addUEdge(graph, 2, 3);
        URF_addUEdge(graph, 3, 4);
        URF_addUEdge(graph, 4, 5);
        URF_addUEdge(graph, 4, 6);
        URF_addUEdge(graph, 5, 7);
        URF_addUEdge(graph, 6, 7);
        URF_addUEdge(graph, 7, 0);
        bondCount = 10;
    }
    else
    {
        graph = readInAdjLists(argv[1]);
        bondCount = graph->E; /*this operation is not part of the interface*/
    }

    /* calculate Unique Ring Families */
    URF_data = URF_calculate(graph);
    
    /* some output */
    URFcount = URF_giveNumber(URF_data);
    printf("==========================================================URF=\n");
    printf("Number of Unique Ring Families: %d\n\n", URFcount);
    for(idx=0; idx<URFcount; ++idx)
    {
        printf("URF %d has weight %d.\n", idx, URF_giveWeight(URF_data, idx));
    }
    /* some more output which might change when the order of the input is changed*/
    if(argc > 2)
    {
        printf("\n===============================================================\n");
        printf("The rest of this output might depend on the order of the input:\n\n");
        for(idx=0; idx<URFcount; ++idx)
        {
            count = URF_giveBonds(URF_data, idx, &bondArray);
            printf("There are %d bonds in URF %d.\n", count, idx);
            free(bondArray);
        }
        printf("\n");
        
        for(idx=0; idx<URFcount; ++idx)
        {
            printf("Atoms in URF %d: ",idx);
            count = URF_giveAtoms(URF_data, idx, &atoms);
            for(obIdx=0; obIdx<count; ++obIdx)
            {
                printf("%d ",atoms[obIdx]);
            }
            printf("\n");
            free(atoms);
        }
        printf("\n");
        
        printf("A possible MCB (SSSR) ");
        count = findBasis(URF_data, &cycleArray);
        printf("(%d rings):\n",count);
        for(idx=0; idx<count; ++idx)
        {
            printf("ring %d: ",idx);
            for(obIdx=0; obIdx<cycleArray[idx].weight;  ++obIdx)
            {
                printf("(%d ",cycleArray[idx].bonds[obIdx][0]);
                printf("%d), ",cycleArray[idx].bonds[obIdx][1]);
            }
            printf("\n");
        }
        deleteCycles(cycleArray, count);
        printf("\n");
        
        printf("The RC Prototypes with bonds as pairs of atoms ");
        count = URF_giveRCPrototypes(URF_data, &cycleArray);
        printf("(%d rings):\n",count);
        for(idx=0; idx<count; ++idx)
        {
            printf("ring %d: ",idx);
            for(obIdx=0; obIdx<cycleArray[idx].weight; ++obIdx)
            {
                printf("(%d ",cycleArray[idx].bonds[obIdx][0]);
                printf("%d), ",cycleArray[idx].bonds[obIdx][1]);
            }
            printf("\n");
        }
        printf("\n");
        
        printf("The RC Prototypes as arrays ");
        count = URF_translateCycArray(URF_data, cycleArray, count, &otherCycleArray);
        printf("(%d rings):\n",count);
        for(idx=0; idx<count; ++idx)
        {
            printf("ring %d: ",idx);
            for(obIdx=0; obIdx<bondCount; ++obIdx)
            {
                printf("%d ",otherCycleArray[idx][obIdx]);
            }
            printf("\n");
        }
        deleteCycles(cycleArray, count);
        URF_deleteEdgeIdxArray(otherCycleArray, count);
    }
    printf("==========================================================URF=\n");

    /* delete URF_data and the graph */
    URF_deleteData(URF_data);
    
    return 0;
}
