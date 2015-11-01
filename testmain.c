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
  URF_atom *atoms;
  URF_bond *bondArray;
  URF_cycle *cycleArray;
  char **otherCycleArray;

  /* read in the graph */
  unsigned int bondArr[][2]={
  {1, 13},
  {2, 17},
  {3,  4},
  {3,  7},
  {3, 13},
  {4, 12},
  {4, 17},
  {5, 13},
  {5, 17},
  {5, 20},
  {6,  7},
  {6,  9},
  {6, 10},
  {6, 12},
  {7,  8},
  {7, 14},
  {8,  9},
  {8, 11},
  {8, 15},
  {9, 16},
  {9, 26},
 {10, 11},
 {10, 18},
 {11, 19},
 {20, 21},
 {20, 22},
 {21, 23},
 {22, 24},
 {23, 25},
 {24, 25}
 };
  atomCount = 26;
  bondCount = 30;
  graph = URF_initNewGraph(atomCount);
  for(idx=0;idx<bondCount; ++idx)
  {
    URF_addUEdge(graph, bondArr[idx][0]-1, bondArr[idx][1]-1);
  }
  /*atomCount = 8;
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
  URF_addUEdge(graph, 7, 0);*/

  /* calculate Unique Ring Families */
  URFdata = URF_calculate(graph);
  
  /* some output */
  URFcount = URF_getNumber(URFdata);
  printf("==========================================================URF=\n");
  printf("Number of Unique Ring Families: %d\n\n", URFcount);
  for(idx=0; idx<URFcount; ++idx)
  {
    printf("URF %d has weight %d.\n", idx, URF_getWeight(URFdata, idx));
  }
  /* some more output which might change when the order of the input is changed*/
  if(argc > 1)
  {
    printf("\n===============================================================\n");
    printf("The rest of this output might depend on the order of the input:\n\n");
    for(idx=0; idx<URFcount; ++idx)
    {
        count = URF_getBonds(URFdata, idx, &bondArray);
        printf("There are %d bonds in URF %d.\n", count, idx);
        free(bondArray);
    }
    printf("\n");
    
    for(idx=0; idx<URFcount; ++idx)
    {
        printf("Atoms in URF %d: ",idx);
        count = URF_getAtoms(URFdata, idx, &atoms);
        for(obIdx=0; obIdx<count; ++obIdx)
        {
            printf("%d ",atoms[obIdx]);
        }
        printf("\n");
        free(atoms);
    }
    printf("\n");
    
    printf("A possible MCB (SSSR) ");
    count = URF_getBasis(URFdata, &cycleArray);
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
    count = URF_getRCPrototypes(URFdata, &cycleArray);
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
    
    printf("The RC Prototypes as vectors ");
    count = URF_translateCycArray(URFdata, cycleArray, count, &otherCycleArray);
    printf("(%d rings):\n",count);
    /* To be able to understand the bitsets better: */
    bondCount = URF_getEdgeArray(URFdata, &bondArray);
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
