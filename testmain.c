/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "URF.h"

int main(int argc, char **argv)
{
    GraphURF *graph;
    urfdata *URFdata;
    int idx, obIdx, URFcount, count, bondCount, atomCount;
    int *atoms;
    int **bondArray;
    int ***cycleArray;
    char **otherCycleArray;

    /* read in the graph */
    atomCount = 8;
    bondCount = 10;
    graph = initNewURFGraph(atomCount);
    addUEdgeURF(graph, 0, 1);
    addUEdgeURF(graph, 0, 2);
    addUEdgeURF(graph, 1, 3);
    addUEdgeURF(graph, 2, 3);
    addUEdgeURF(graph, 3, 4);
    addUEdgeURF(graph, 4, 5);
    addUEdgeURF(graph, 4, 6);
    addUEdgeURF(graph, 5, 7);
    addUEdgeURF(graph, 6, 7);
    addUEdgeURF(graph, 7, 0);

    /* calculate Unique Ring Families */
    URFdata = calculateURFs(graph);
    
    /* some output */
    URFcount = numberOfURFs(URFdata);
    printf("==========================================================URF=\n");
    printf("Number of Unique Ring Families: %d\n\n", URFcount);
    for(idx=0; idx<URFcount; ++idx)
    {
        printf("URF %d has weight %d.\n", idx, giveURFWeight(URFdata, idx));
    }
    /* some more output which might change when the order of the input is changed*/
    if(argc > 1)
    {
        printf("\n===============================================================\n");
        printf("The rest of this output might depend on the order of the input:\n\n");
        for(idx=0; idx<URFcount; ++idx)
        {
            count = giveURFBonds(URFdata, idx, &bondArray);
            printf("There are %d bonds in URF %d.\n", count, idx);
            deleteBondArr(bondArray);
        }
        printf("\n");
        
        for(idx=0; idx<URFcount; ++idx)
        {
            printf("Atoms in URF %d: ",idx);
            count = giveURFAtoms(URFdata, idx, &atoms);
            for(obIdx=0; obIdx<count; ++obIdx)
            {
                printf("%d ",atoms[obIdx]);
            }
            printf("\n");
            free(atoms);
        }
        printf("\n");
        
        printf("A possible MCB (SSSR) ");
        count = findBasis(URFdata, &cycleArray);
        printf("(%d rings):\n",count);
        for(idx=0; idx<count; ++idx)
        {
            printf("ring %d: ",idx);
            for(obIdx=0; cycleArray[idx][obIdx]!=NULL;  ++obIdx)/*the end of a cycle is signaled by a NULL pointer*/
            {
                printf("(%d ",cycleArray[idx][obIdx][0]);
                printf("%d), ",cycleArray[idx][obIdx][1]);
            }
            printf("\n");
        }
        deleteCycles(cycleArray, count);
        printf("\n");
        
        printf("The RC Prototypes with bonds as pairs of atoms ");
        count = giveRCprototypes(URFdata, &cycleArray);
        printf("(%d rings):\n",count);
        for(idx=0; idx<count; ++idx)
        {
            printf("ring %d: ",idx);
            for(obIdx=0; cycleArray[idx][obIdx]!=NULL; ++obIdx)/*the end of a cycle is signaled by a NULL pointer*/
            {
                printf("(%d ",cycleArray[idx][obIdx][0]);
                printf("%d), ",cycleArray[idx][obIdx][1]);
            }
            printf("\n");
        }
        printf("\n");
        
        printf("The RC Prototypes as arrays ");
        count = translateCycleArray(URFdata, cycleArray, count, &otherCycleArray);
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
        deleteEdgeIdxCycles(otherCycleArray);
    }
    printf("==========================================================URF=\n");

    /* delete urfdata and the graph */
    deleteURFdata(URFdata);
}
