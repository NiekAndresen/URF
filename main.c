/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "graphURF.h"
#include "utility.h"
#include "URF.h"

int main(int argc, char **argv)
{
    GraphURF graph;
    
    readInList(&graph, "adjlist");
    printGraph(&graph);
    
    urfdata *udata = calculateURFs(&graph);
    printf("apsp solved\n");
    printf("dist:\n");
    print2DIntArray(udata->spi->dist, graph.V, graph.V);
    printf("pred:\n");
    print2DIntArray(udata->spi->pred, graph.V, graph.V);
    printf("reachable:\n");
    print2DCharArray(udata->spi->reachable, graph.V, graph.V);
    
    deleteURFdata(udata);
    deleteGraph(&graph);
    printf("Niek ist die Nummer %d\n",1);
    return EXIT_SUCCESS;
}
