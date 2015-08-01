/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graphURF.h"

void prepareEnumeration(GraphURF *gra, int V, int E)
{
    int i;
    int **edges;
    int *startIdxEdges;
    edges = malloc(E * sizeof(*edges));
    for(i=0; i<E; ++i)
    {
        edges[i] = malloc(2 * sizeof(**edges));
    }
    gra->edges = edges;
    
    startIdxEdges = malloc(V * sizeof(*startIdxEdges));
    gra->startIdxEdges = startIdxEdges;
}

void initGraph(GraphURF *gra, int V, int E, int *degree, char enumerateEdges)
{
    int i;
    int **adjList;
    
    gra->V = V;
    gra->E = E;
    gra->degree = degree;
    
    adjList = malloc(V * sizeof(*adjList));
    for(i=0; i<V; ++i)
    {
        if(degree[i] > 0)
        {
            adjList[i] = malloc(degree[i] * sizeof(**adjList));
        }
    }
    gra->adjList = adjList;
    
    gra->edgesEnumerated = enumerateEdges;
    if(enumerateEdges == 'y')
    {
        prepareEnumeration(gra, V, E);
    }
}

int isAdj(GraphURF *graph, int i, int j)
{
    int idx;
    for(idx=0; idx<graph->degree[i]; ++idx)
    {
        if(graph->adjList[i][idx] == j)
        {
            return 1;
        }
    }
    return 0;
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
    if(gra->edgesEnumerated == 'y')
    {
        for(i=0; i<gra->E; ++i)
        {
            free(gra->edges[i]);
        }
        free(gra->edges);
        free(gra->startIdxEdges);
    }
    free(gra->degree);
    free(gra);
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
    if(graph->edgesEnumerated == 'y')
    {
        printf("edges:\n");
        for(i=0; i<graph->E; ++i)
        {
            printf("%d: [%d,%d]\n", i, graph->edges[i][0], graph->edges[i][1]);
        }
    }
    printf("\n");
}

GraphURF *initNewGraph(int V)
{
    GraphURF *graph = malloc(sizeof(*graph));
    int *degree;
    int i;
    degree = malloc(V * sizeof(*degree));
    for(i=0; i<V; ++i)
    {
        degree[i] = 0;
    }
    initGraph(graph,V,0,degree,'n');
    return graph;
}

void addEdge(GraphURF *gra, int from, int to)
{
    ++gra->E;
    ++gra->degree[from];
    if(gra->degree[from] == 1)/*has never been initialized*/
    {
        gra->adjList[from] = malloc(gra->degree[from] * sizeof(*gra->adjList[from]));
    }
    else
    {
        gra->adjList[from] = realloc(gra->adjList[from], gra->degree[from] * sizeof(*gra->adjList[from]));
    }
    gra->adjList[from][ gra->degree[from]-1 ] = to;
}

void addUEdge(GraphURF *gra, int from, int to)
{
    addEdge(gra, from, to);
    addEdge(gra, to, from);
    --gra->E; /*was incremented twice*/
}

void enumerateEdges(GraphURF *gra)
{
    int ed=0,li,ve;
    if(gra->edgesEnumerated != 'y') /*Enumeration has not been prepared before.*/
    {
        prepareEnumeration(gra, gra->V, gra->E);
    }
    gra->edgesEnumerated = 'y';
    //read over all of the adjLists
    for(li=0; li<gra->V; ++li)
    {
        gra->startIdxEdges[li] = ed; 
        for(ve=0; ve<gra->degree[li]; ++ve)
        {
            if(li < gra->adjList[li][ve])
            {
                gra->edges[ed][0] = li;
                gra->edges[ed][1] = gra->adjList[li][ve];
                ++ed;
            }
        }
    }
}
