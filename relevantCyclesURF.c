#include "apsp.h"
#include "graphURF.h"
#include "relevantCyclesURF.h"

void vismara(rcURF *rc, GraphURF *gra, sPathInfo *spi)
{
    
}

rcURF *findRelCycles(GraphURF *gra, sPathInfo *spi)
{
    rcURF *rc = malloc(sizeof(*rc));
    vismara(rc, gra, spi);
    return rc;
}

void deleteRelCycles(rcURF *rc)
{
    free(rc);
}

