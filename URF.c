/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "URF.h"
#include "apsp.h"
#include "CycleFamsURF.h"

void findShortestPaths(urfdata *udata, GraphURF *gra)
{
    udata->spi = AllPairsShortestPaths(gra);
}

int countURFs(cfURF *cFams, URFinfo *uInfo)
{
    int RCFcount=0, URFRelCount=0, weightIdx, i, j;
    /*count number of cycles families that are marked as relevant*/
    for(i=0; i<cFams->nofFams; ++i)
    {
        if(cFams->fams[i]->mark > 0)
        {
            ++RCFcount;
        }
    }
    /*count number of 1s indicating URF-relation*/
    for(weightIdx=0; weightIdx<uInfo->nofWeights; ++weightIdx)
    {
        for(i=0; i<uInfo->nofProtos[weightIdx]; ++i)
        {
            for(j=i+1; j<uInfo->nofProtos[weightIdx]; ++j)
            {
                if(uInfo->URFs[weightIdx][i][j] == 1)
                {
                    ++URFRelCount;
                }
            }
        }
    }
    return RCFcount - URFRelCount;
}

urfdata *calculateURFs(GraphURF *gra)
{
    urfdata *udata = malloc(sizeof(*udata));
    findShortestPaths(udata, gra);
    udata->CFs = findCycleFams(gra, udata->spi);
    udata->graph = gra;
    udata->urfInfo = checkURFRelation(udata->CFs, udata->graph, udata->spi);
    udata->nofURFs = countURFs(udata->CFs, udata->urfInfo);
    return udata;
}

void deleteURFdata(urfdata *udata)
{
    deleteAPSP(udata->spi, udata->graph->V);
    deleteCycleFams(udata->CFs);
    deleteURFInfo(udata->urfInfo);
    free(udata);
}
