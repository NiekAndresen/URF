#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

void initGraph(Graph *gra, int V, int E, int *degree)
{
    int i;
    int **adjList;
    
    gra->V = V;
    gra->E = E;
    gra->degree = degree;
    
    adjList = malloc(V * sizeof(*adjList));
    for(i=0; i<V; ++i)
    {
        adjList[i] = malloc(degree[i] * sizeof(**adjList));
    }
    gra->adjList = adjList;
}

void deleteGraph(Graph *gra)
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
    free(gra->degree);
}

void printGraph(Graph *graph)
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
}
