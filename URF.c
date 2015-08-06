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
