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
    int i,j;
    
    readInList(&graph, "adjlist");
    printGraph(&graph);
    
    urfdata *udata = calculateURFs(&graph);
    
    printf("calculated\n");
    printf("dist:\n");
    print2DIntArray(udata->spi->dist, graph.V, graph.V);
    printf("pred:\n");
    print2DIntArray(udata->spi->pred, graph.V, graph.V);
    printf("reachable:\n");
    print2DCharArray(udata->spi->reachable, graph.V, graph.V);
    printf("RCFs:\n");
    printf("Number of Fams: %d\n", udata->RCFs->nofFams);
    for(i=0; i<udata->RCFs->nofFams; ++i)
    {
        printf("a prototype: ");
        for(j=0; j<graph.E; ++j)
        {
            printf(" %d ", udata->RCFs->fams[i]->prototype[j]);
        }
        printf("\n");
    }
    
    deleteURFdata(udata);
    deleteGraph(&graph);
    printf("Niek ist die Nummer %d\n",1);
    return EXIT_SUCCESS;
}
