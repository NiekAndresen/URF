/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "URFInfo.h"
#include "URFrelation.h"

/** allocates the matrices in urfInfo */
URFinfo *URF_initUrfInfo(cfURF *RCFs, URF_graph *graph)
{
  URFinfo *urfInfo;
  char ***URFrel;
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
  URFrel = malloc(numOfWeights * sizeof(*URFrel));
  for(i=0; i<numOfWeights; ++i)
  {
    URFrel[i] = malloc(numOfProtos[i] * sizeof(*URFrel[i]));
    *URFrel[i] = malloc(numOfProtos[i] * numOfProtos[i] * sizeof(**URFrel[i]));
    for(j=1; j<numOfProtos[i]; ++j)
    {
      URFrel[i][j] = URFrel[i][j-1] + numOfProtos[i];
    }
  }
  
  /*initialize with zeros*/
  for(i=0; i<numOfWeights; ++i)
  {
    for(j=0; j<numOfProtos[i]; ++j)
    {
      for(k=0; k<numOfProtos[i]; ++k)
      {
        URFrel[i][j][k] = 0;
      }
    }
  }
  
  urfInfo->nofWeights = numOfWeights;
  urfInfo->nofProtos = numOfProtos;
  urfInfo->URFrel = URFrel;
  
  return urfInfo;
}

void URF_deleteURFInfo(URFinfo *uInfo)
{
  int i;
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    free(uInfo->URFrel[i][0]);
    free(uInfo->URFrel[i]);
  }
  free(uInfo->URFrel);
  free(uInfo->nofProtos);
  for(i=0; i<uInfo->nofURFs; ++i)
  {
    free(uInfo->URFs[i]);
  }
  free(uInfo->URFs);
  free(uInfo->nofCFsPerURF);
  free(uInfo);
}

/** returns the number of URFs. */
int URF_countURFs(URFinfo *uInfo)
{
  int URFRelCount=0, weightIdx, i, j;
  char **alreadyInURF; /*for each weight: an array storing if a RCF is already
                        part of a URF*/
  char increase;
  
  alreadyInURF = malloc(uInfo->nofWeights * sizeof(*alreadyInURF));
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    alreadyInURF[i] = malloc(uInfo->nofProtos[i] * sizeof(*alreadyInURF[i]));
  }
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    for(j=0; j<uInfo->nofProtos[i]; ++j)
    {
      alreadyInURF[i][j] = 0;
    }
  }
  
  /*count number of 1s indicating URF-relation*/
  for(weightIdx=0; weightIdx<uInfo->nofWeights; ++weightIdx)
  {
    for(i=0; i<uInfo->nofProtos[weightIdx]; ++i)
    {
      if(alreadyInURF[weightIdx][i] == 1 || uInfo->URFrel[weightIdx][i][i] == 0) 
      {
        continue;
      }
      increase = 0;
      for(j=i; j<uInfo->nofProtos[weightIdx]; ++j)
      {
        if(uInfo->URFrel[weightIdx][i][j] == 1)
        {
          alreadyInURF[weightIdx][j] = 1;
          if(increase == 0)
          {
            ++URFRelCount;
            increase = 1;
          }
        }
      }
    }
  }
  
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    free(alreadyInURF[i]);
  }
  free(alreadyInURF);
  return URFRelCount;
}

/** returns the index in the array of RCFs (fams) that the RCF with the weight
at the index "weight" and position j has */
int URF_idxWeight(URFinfo *uInfo, int weight, int j)
{
  int i, sum = 0;
  for(i=0; i<weight; ++i)
  {
    sum += uInfo->nofProtos[i];
  }
  return sum + j;
}

/** adds the CF with the index 'RCFIdx' to the URF with the index 'URFIdx'.
sNeeds the number of RCFs already in the URF as third parameter
'previouslyAlloced' */
void URF_addRCFtoURF(int RCFIdx, int URFIdx, int previouslyAlloced,
                     cfam ***URFs, cfURF *CFs)
{
  if(previouslyAlloced == 0)
  {
    URFs[URFIdx] = malloc((previouslyAlloced+1) * sizeof(*URFs[URFIdx]));
  }
  else
  {
    URFs[URFIdx] = realloc(URFs[URFIdx], (previouslyAlloced+1) *
                           sizeof(*URFs[URFIdx]));
  }
  URFs[URFIdx][previouslyAlloced] =  CFs->fams[RCFIdx];
}

void URF_fillURFs(URFinfo *uInfo, cfURF *CFs)
{
  cfam ***URFs;
  int *nofRCFs;
  int currURFIdx=0;
  int i,j,k;
  char **alreadyInURF; /*for each weight: an array storing if a RCF is already
                        part of a URF*/
  
  alreadyInURF = malloc(uInfo->nofWeights * sizeof(*alreadyInURF));
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    alreadyInURF[i] = malloc(uInfo->nofProtos[i] * sizeof(*alreadyInURF[i]));
  }
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    for(j=0; j<uInfo->nofProtos[i]; ++j)
    {
      alreadyInURF[i][j] = 0;
    }
  }
  
  URFs = malloc(uInfo->nofURFs * sizeof(*URFs));
  nofRCFs = malloc(uInfo->nofURFs * sizeof(*nofRCFs));
  for(i=0; i<uInfo->nofURFs; ++i)
  {
    nofRCFs[i] = 0;
  }
  
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    for(j=0; j<uInfo->nofProtos[i]; ++j)
    {
      /*if this family is not already part of a URF*/
      if((uInfo->URFrel[i][j][j] == 1) && (alreadyInURF[i][j] == 0))
      {
        for(k=j; k<uInfo->nofProtos[i]; ++k)
        {
          if(uInfo->URFrel[i][j][k] == 1)
          {
            /*add to current URF the CFs with the given Idx*/
            URF_addRCFtoURF(URF_idxWeight(uInfo,i,k),currURFIdx,
                            nofRCFs[currURFIdx],URFs,CFs);
            ++nofRCFs[currURFIdx];
            alreadyInURF[i][k] = 1;
          }
        }
        ++currURFIdx;
      }
    }
  }
  
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    free(alreadyInURF[i]);
  }
  free(alreadyInURF);
  uInfo->URFs=URFs;
  uInfo->nofCFsPerURF = nofRCFs;
}

URFinfo *URF_checkURFRelation(cfURF *RCFs, URF_graph *graph, sPathInfo *spi)
{
  URFinfo *uInfo = URF_initUrfInfo(RCFs, graph);
  URF_findRelations(RCFs, graph, uInfo, spi);
  
  uInfo->nofURFs = URF_countURFs(uInfo);
  URF_fillURFs(uInfo, RCFs);
  return uInfo;
}

