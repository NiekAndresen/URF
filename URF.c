/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "URF.h"
#include "apsp.h"
#include "CycleFamsURF.h"
#include "URFhandler.h"

void findShortestPaths(urfdata *udata, GraphURF *gra)
{
    udata->spi = AllPairsShortestPaths(gra);
}

urfdata *calculateURFs(GraphURF *gra)
{
    urfdata *udata = malloc(sizeof(*udata));
    findShortestPaths(udata, gra);
    udata->CFs = findCycleFams(gra, udata->spi);
    udata->graph = gra;
    udata->urfInfo = checkURFRelation(udata->CFs, udata->graph, udata->spi);
    udata->nofURFs = udata->urfInfo->nofURFs;
    return udata;
}

void deleteURFdata(urfdata *udata)
{
    deleteAPSP(udata->spi, udata->graph->V);
    deleteCycleFams(udata->CFs);
    deleteURFInfo(udata->urfInfo);
    free(udata);
}

int numberOfURFs(urfdata *udata)
{
    return udata->nofURFs;    
}

/** gives an array of indices of atoms that are contained in the URF with the given index. Array is terminated by INT_MAX */
int *giveAtoms(urfdata *uData, int index)
{
    int i,nofFams,nextfree=0,alloced;
    char *atoms;
    int *result;
    
    nofFams = uData->urfInfo->nofCFsPerURF[index];
    cfam **URF = uData->urfInfo->URFs[index];
    atoms = malloc(uData->graph->V * sizeof(*atoms));
    result = malloc(8 * sizeof(*result));
    alloced = 8;
    
    for(i=0; i<uData->graph->V; ++i)
    {
        atoms[i] = 0;
    }
    for(i=0; i<nofFams; ++i)
    {
        giveVertices(URF[i]->r, URF[i]->q, atoms, uData->graph, uData->spi);
        giveVertices(URF[i]->r, URF[i]->p, atoms, uData->graph, uData->spi);
        if(URF[i]->x < INT_MAX) /*even cycle*/
        {
            atoms[URF[i]->x] = 1;
        }
    }
    
    for(i=0; i<uData->graph->V; ++i)
    {
        if(atoms[i] == 1)
        {
            if(nextfree == alloced)
            {/*double the size*/
                result = realloc(result, 2*alloced*sizeof(*result));
                alloced *= 2;
            }
            result[nextfree++] = i;
        }
    }
    if(nextfree == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[nextfree] = INT_MAX;
    
    free(atoms);
    return result;
}

/** gives an array of indices of bonds that are contained in the URF with the given index. Array is terminated by INT_MAX */
int *giveBonds(urfdata *uData, int index)
{
    int i,nofFams,nextfree=0,alloced;
    char *bonds;
    int *result;
    
    nofFams = uData->urfInfo->nofCFsPerURF[index];
    cfam **URF = uData->urfInfo->URFs[index];
    bonds = malloc(uData->graph->E * sizeof(*bonds));
    result = malloc(8 * sizeof(*result));
    alloced = 8;
    
    for(i=0; i<uData->graph->E; ++i)
    {
        bonds[i] = 0;
    }
    for(i=0; i<nofFams; ++i)
    {
        giveEdges(URF[i]->r, URF[i]->q, bonds, uData->graph, uData->spi);
        giveEdges(URF[i]->r, URF[i]->p, bonds, uData->graph, uData->spi);
        if(URF[i]->x < INT_MAX) /*even cycle*/
        {
            bonds[edgeIdx(URF[i]->q,URF[i]->x,uData->graph)] = 1;
            bonds[edgeIdx(URF[i]->p,URF[i]->x,uData->graph)] = 1;
        }
        else/*odd cycle*/
        {
            bonds[edgeIdx(URF[i]->q,URF[i]->p,uData->graph)] = 1;
        }
    }
    
    for(i=0; i<uData->graph->E; ++i)
    {
        if(bonds[i] == 1)
        {
            if(nextfree == alloced)
            {/*double the size*/
                result = realloc(result, 2*alloced*sizeof(*result));
                alloced *= 2;
            }
            result[nextfree++] = i;
        }
    }
    if(nextfree == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[nextfree] = INT_MAX;
    
    free(bonds);
    return result;
}

int *giveURF(urfdata *uData, int URFindex, char mode)
{
    int *result;
    if(mode == 'a')
    {
        result = giveAtoms(uData, URFindex);
    }
    else if(mode == 'b')
    {
        result = giveBonds(uData, URFindex);
    }
    else
    {
        fprintf(stderr, "ERROR: tried to call 'giveURF()' with invalid mode '%c'\n",mode);
        fprintf(stderr, "Only 'a' or 'b' are allowed. Read interface for help.\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

char **giveURFCycles(urfdata *udata, int index)
{
    int i;
    int currIdx=0; /*index of next free space in result array*/
    int alloced=2; /*space for how many cycles/paths has been allocated; is always a power of two. =2 in the beginning, grows with number of cycles*/
    int nofFams;
    cfam **URF;
    char **paths1, **paths2; /*much like the result array, but storing paths instead of cycles. paths1 for paths from r to p and paths2 for paths from r to q.*/
    char **result;
    
    nofFams = udata->urfInfo->nofCFsPerURF[index];
    URF = udata->urfInfo->URFs[index];
    
    result = malloc(alloced * sizeof(*result));
    for(i=0; i<nofFams; ++i)
    {
        getPaths(URF[i]->r, URF[i]->p, &paths1, alloced, udata->graph, udata->spi);
        getPaths(URF[i]->r, URF[i]->q, &paths2, alloced, udata->graph, udata->spi);
        currIdx = combinePaths(&paths1, &paths2, URF[i]->x, &result, currIdx, alloced, udata->graph);
        /*calculate how many spaces are alloced with the help of the returned currIdx. Can be done since 'alloced' is always a power of two*/
        while(currIdx > alloced) alloced *= 2;
    }
    if(currIdx == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[currIdx] = NULL;
    return result;
}

void deleteURFCycles(char **cycles)
{
    int i;
    char *cyc;
    cyc = cycles[0];
    for(i=0; cyc!=NULL; ++i)
    {
        free(cyc);
        cyc = cycles[i+1];
    }
    free(cycles);
}
