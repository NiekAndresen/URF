/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "URFInfo.h"
#include "URFrelation.h"

/** allocates the matrices in urfInfo */
URFinfo *initUrfInfo(cfURF *RCFs, GraphURF *graph)
{
    URFinfo *urfInfo;
    int ***URFs;
    int numOfWeights = 1;
    int *numOfProtos;
    int i, j, k, currWeight, currIdx;
    urfInfo = malloc(sizeof(*urfInfo));
    
    /* count number of different weights occuring*/
    currWeight = RCFs->fams[0]->weight;
    for(i=0; i<RCFs->nofFams; ++i)
    {
        if(RCFs->fams[i]->weight != currWeight)
        {
            ++numOfWeights;
            currWeight = RCFs->fams[i]->weight;
        }
    }
    
    /* for each weight count number of different prototypes */
    numOfProtos = malloc(numOfWeights * sizeof(*numOfProtos));
    for(i=0; i<numOfWeights; ++i)
    {
        numOfProtos[i] = 0;
    }
    currWeight = RCFs->fams[0]->weight;
    currIdx = 0;
    for(i=0; i<RCFs->nofFams; ++i)
    {
        if(RCFs->fams[i]->weight != currWeight)
        {
            ++currIdx;
            currWeight = RCFs->fams[i]->weight;
        }
        ++numOfProtos[currIdx];
    }
    
     /*allocate everything*/
    URFs = malloc(numOfWeights * sizeof(*URFs));
    for(i=0; i<numOfWeights; ++i)
    {
        URFs[i] = malloc(numOfProtos[i] * sizeof(*URFs[i]));
        *URFs[i] = malloc(numOfProtos[i] * numOfProtos[i] * sizeof(**URFs[i]));
        for(j=1; j<numOfProtos[i]; ++j)
        {
            URFs[i][j] = URFs[i][j-1] + numOfProtos[i];
        }
    }
    
    /*initialize with zeros*/
    for(i=0; i<numOfWeights; ++i)
    {
        for(j=0; j<numOfProtos[i]; ++j)
        {
            for(k=0; k<numOfProtos[i]; ++k)
            {
                URFs[i][j][k] = 0;
            }
        }
    }
    
    urfInfo->nofWeights = numOfWeights;
    urfInfo->nofProtos = numOfProtos;
    urfInfo->URFs = URFs;
    
    return urfInfo;
}

void deleteURFInfo(URFinfo *uInfo)
{
    int i;
    for(i=0; i<uInfo->nofWeights; ++i)
    {
        free(uInfo->URFs[i][0]);
        free(uInfo->URFs[i]);
    }
    free(uInfo->URFs);
    free(uInfo->nofProtos);
    free(uInfo);
}

URFinfo *checkURFRelation(cfURF *RCFs, GraphURF *graph, sPathInfo *spi)
{
    URFinfo *uInfo = initUrfInfo(RCFs, graph);
    findRelations(RCFs, graph, uInfo, spi);
    return uInfo;
}

