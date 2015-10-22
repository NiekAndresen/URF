/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "URF.h"

/* Simple program showing some information about URFs of a given graph.
This program does not take any specific command line arguments.
If you give any number (>0) of command line arguments, the program
will give some more information. */

int main(int argc, char **argv)
{
    URF_graph *graph;
    URF_data *URFdata;
    int idx, obIdx, URFcount, count, bondCount, atomCount;
    URFAtom *atoms;
    URFBond *bondArray;
    URFCycle *cycleArray;
    char **otherCycleArray;

    /* read in the graph */
    atomCount = 8;
    bondCount = 10;
    graph = URF_initNewGraph(atomCount);
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

    /* calculate Unique Ring Families */
    URFdata = URF_calculate(graph);
    
    /* some output */
    URFcount = URF_giveNumber(URFdata);
    printf("==========================================================URF=\n");
    printf("Number of Unique Ring Families: %d\n\n", URFcount);
    for(idx=0; idx<URFcount; ++idx)
    {
        printf("URF %d has weight %d.\n", idx, URF_giveWeight(URFdata, idx));
    }
    /* some more output which might change when the order of the input is changed*/
    if(argc > 1)
    {
        printf("\n===============================================================\n");
        printf("The rest of this output might depend on the order of the input:\n\n");
        for(idx=0; idx<URFcount; ++idx)
        {
            count = URF_giveBonds(URFdata, idx, &bondArray);
            printf("There are %d bonds in URF %d.\n", count, idx);
            free(bondArray);
        }
        printf("\n");
        
        for(idx=0; idx<URFcount; ++idx)
        {
            printf("Atoms in URF %d: ",idx);
            count = URF_giveAtoms(URFdata, idx, &atoms);
            for(obIdx=0; obIdx<count; ++obIdx)
            {
                printf("%d ",atoms[obIdx]);
            }
            printf("\n");
            free(atoms);
        }
        printf("\n");
        
        printf("A possible MCB (SSSR) ");
        count = URF_findBasis(URFdata, &cycleArray);
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
        URF_deleteCycles(cycleArray, count);
        printf("\n");
        
        printf("The RC Prototypes with bonds as pairs of atoms ");
        count = URF_giveRCPrototypes(URFdata, &cycleArray);
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
        count = URF_translateCycArray(URFdata, cycleArray, count, &otherCycleArray);
        printf("(%d rings):\n",count);
        /* To be able to understand the bitsets better: */
        bondCount = URF_giveEdgeArray(URFdata, &bondArray);
        printf("Edge from");
        for(idx=0; idx<bondCount; ++idx)
        {
            printf("%2d",bondArray[idx][0]);
        }
        printf("\n     to  ");
        for(idx=0; idx<bondCount; ++idx)
        {
            printf("%2d",bondArray[idx][1]);
        }
        printf("\n");
        free(bondArray);
        /* the bitsets: */
        for(idx=0; idx<count; ++idx)
        {
            printf("ring %3d: ",idx);
            for(obIdx=0; obIdx<bondCount; ++obIdx)
            {
                printf("%d ",otherCycleArray[idx][obIdx]);
            }
            printf("\n");
        }
        URF_deleteCycles(cycleArray, count);
        URF_deleteEdgeIdxArray(otherCycleArray, count);
    }
    printf("==========================================================URF=\n");

    /* delete URFdata and the graph */
    URF_deleteData(URFdata);
    return 0;
}
