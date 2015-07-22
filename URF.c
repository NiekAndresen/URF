/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include "URF.h"
#include "apsp.h"
#include "relevantCyclesURF.h"

void findShortestPaths(urfdata *udata, GraphURF *gra)
{
    udata->spi = AllPairsShortestPaths(gra);
}

urfdata *calculateURFs(GraphURF *gra)
{
    urfdata *udata = malloc(sizeof(*udata));
    findShortestPaths(udata, gra);
    udata->RCFs = findRelCycles(gra, udata->spi);
    return udata;
}

void deleteURFdata(urfdata *udata)
{
    deleteAPSP(udata->spi);
    deleteRelCycles(udata->RCFs);
    free(udata);
}
