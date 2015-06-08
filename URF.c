/** Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "graph.h"
#include "apsp.h"
#include "utility.h"

int main(int argc, char **argv)
{
    Graph graph;
    
    readInList(&graph, "adjlist");
    printGraph(&graph);
    
    sPathInfo *shortest = AllPairsShortestPaths(&graph);
    printf("apsp solved\n");
    printf("dist:\n");
    print2DIntArray(shortest->dist, graph.V, graph.V);
    printf("pred:\n");
    print2DIntArray(shortest->pred, graph.V, graph.V);
    printf("reachable:\n");
    print2DCharArray(shortest->reachable, graph.V, graph.V);
    
    deleteAPSP(shortest);
    deleteGraph(&graph);
    printf("Niek ist die Nummer %d\n",1);
    return EXIT_SUCCESS;
}
