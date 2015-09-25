/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graphURF.h"
#include "utility.h"

/** allocates space for the arrays edges and startIdxEdges */
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

/** initializes a new graph with the values |V|, |E| and the array degree, then
    allocates enough space for 'adjList' and 'edges', so that they can be filled.
    Can be called with E=0 to be able to addEdges later.*/
void initGraph(GraphURF *gra, int V, int E, int *degree)
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
    
    gra->edgesEnumerated = 'n';
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
    int i;
    assert(gra != NULL);
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
    initGraph(graph,V,0,degree);
    return graph;
}

void addEdge(GraphURF *gra, int from, int to)
{
    int i;
    for(i=0; i<gra->degree[from]; ++i)
    {
        if(gra->adjList[from][i] == to)
        {/*edge already exists*/
            return;
        }
    }
    ++gra->E;
    ++gra->degree[from];
    if(gra->degree[from] == 1)/*was 0, has never been initialized*/
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
    int i;
    for(i=0; i<gra->degree[from]; ++i)
    {
        if(gra->adjList[from][i] == to)
        {/*edge already exists*/
            return;
        }
    }
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
        gra->edgesEnumerated = 'y';
    }
    /*read over all of the adjLists*/
    for(li=0; li<gra->V; ++li)
    {
        gra->startIdxEdges[li] = ed;
        for(ve=0; ve<gra->degree[li]; ++ve)
        {
            if(li < gra->adjList[li][ve])/*to count every edge only once*/
            {
                gra->edges[ed][0] = li;
                gra->edges[ed][1] = gra->adjList[li][ve];
                ++ed;
            }
        }
    }
}

void DFSvisit(GraphURF *gra, int vertex, char *visited)
{
    int i,j;
    for(i=0; i<gra->degree[vertex]; ++i)
    {
        j = gra->adjList[vertex][i];
        if(visited[j] == 0)
        {
            visited[j] = 1;
            DFSvisit(gra,j,visited);
        }
    }
}

/** returns 1 if the graph is connected, 0 otherwise. uses DFS */
char checkGraphConnected(GraphURF *gra)
{
    int i;
    char *visited;
    char result;
    result = 1;
    visited = malloc(gra->V * sizeof(*visited));
    visited[0] = 1; /*start vertex*/
    for(i=1; i<gra->V; ++i)
    {
        visited[i] = 0; /*unvisited*/
    }
    DFSvisit(gra, 0, visited);
    for(i=0; i<gra->V; ++i)
    {
        if(visited[i] == 0) /*one was unvisited*/
        {
            result = 0;
            break;
        }
    }
        
    free(visited);
    return result;
}

char checkGraphCorrect(GraphURF *gra)
{
    /*int i,j;
    int from, to;*/
    char edgeUndir;
    char connected;
    connected = 0;
    edgeUndir = 1;
    /*edgeUndir = 0;
    for(from=0; from<gra->V; ++from)
    {
        for(j=0; j<gra->degree[from]; ++j)
        {
            to = gra->adjList[from][j];
            if(from < to)
            {
                edgeUndir = 0;
                for(i=0; i<gra->degree[to]; ++i)
                {
                    if(gra->adjList[to][i] == from)
                    {
                        edgeUndir = 1;
                        break;
                    }
                }
            }
            if(edgeUndir == 0) break;
        }
        if(edgeUndir == 0) break;
    }*/
    
    connected = checkGraphConnected(gra);

    if(edgeUndir == 0 || connected == 0)
    {
        return 0;
    }
    return 1;
}

int edgeId(GraphURF *gra, int from, int to)
{
    int edge;
    char found;
    
    found = 'f';
    if(from > to)
    {/*swap order to make from < to*/
        edge = to;
        to = from;
        from = edge;
    }

    for(edge=gra->startIdxEdges[from]; edge<gra->E; ++edge)
    {
        if((gra->edges[edge][0] == from) && (gra->edges[edge][1] == to))
        {
            found = 't';
            break;
        }
    }
    if(found == 'f')
    {
        fprintf(stderr, "Could not find edge [%d,%d]\n",from,to);
        usage('e');
    }
    return edge;
}
