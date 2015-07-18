#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graphURF.h"

void initGraph(GraphURF *gra, int V, int E, int *degree)
{
    int i;
    int **adjList;
    int **edges;
    
    gra->V = V;
    gra->E = E;
    gra->degree = degree;
    
    adjList = malloc(V * sizeof(*adjList));
    for(i=0; i<V; ++i)
    {
        adjList[i] = malloc(degree[i] * sizeof(**adjList));
    }
    gra->adjList = adjList;
    
    edges = malloc(E * sizeof(*edges));
    for(i=0; i<E; ++i)
    {
        edges[i] = malloc(2 * sizeof(**edges));
    }
    gra->edges = edges;
}

void deleteGraph(GraphURF *gra)
{
    assert(gra != NULL);
    int i;
    for(i=0; i<gra->V; ++i)
    {
        if(gra->degree[i] > 0)
        {
            free(gra->adjList[i]);
        }
    }
    free(gra->adjList);
    for(i=0; i<gra->E; ++i)
    {
        free(gra->edges[i]);
    }
    free(gra->edges);
    free(gra->degree);
}

void printGraph(GraphURF *graph)
{
    int i,j;
    
    printf("|V|=%d, |E|=%d\n",graph->V,graph->E);
    for(i=0; i<graph->V; ++i)
    {
        printf("%d:  ",i);
        for(j=0; j<graph->degree[i]; ++j)
        {
            printf("%d ",graph->adjList[i][j]);
        }
        printf("\n");
    }
    printf("edges:\n");
    for(i=0; i<graph->E; ++i)
    {
        printf("%d: [%d,%d]\n", i, graph->edges[i][0], graph->edges[i][1]);
    }
    printf("\n");
}
