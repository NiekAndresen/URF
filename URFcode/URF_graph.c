/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "URF_graph.h"
#include "utility.h"

/** initializes a new graph with the value |V| and the array degree, then
    allocates enough space for 'adjList', so that they can be filled.
    E is set to 0, edges are added later.*/
void URF_initGraph(URF_graph *gra, int V, int *degree)
{
    int i;
    int **adjList;
    
    gra->V = V;
    gra->E = 0;
    gra->degree = degree;
    gra->edgesAlloced = 4;
    
    adjList = malloc(V * sizeof(*adjList));
    for(i=0; i<V; ++i)
    {
        if(degree[i] > 0)
        {
            adjList[i] = malloc(degree[i] * sizeof(**adjList));
        }
    }
    gra->adjList = adjList;
    
    gra->edges = malloc(gra->edgesAlloced * sizeof(*gra->edges));
}

void URF_DFSvisit(URF_graph *gra, int vertex, char *visited)
{
    int i,j;
    for(i=0; i<gra->degree[vertex]; ++i)
    {
        j = gra->adjList[vertex][i];
        if(visited[j] == 0)
        {
            visited[j] = 1;
            URF_DFSvisit(gra,j,visited);
        }
    }
}

/** returns 1 if the graph is connected, 0 otherwise. uses DFS */
char URF_checkGraphConnected(URF_graph *gra)
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
    URF_DFSvisit(gra, 0, visited);
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

/** returns 1 if the graph is directed, 0 otherwise. */
char URF_checkGraphDirected(URF_graph *gra)
{
    int from,to,j,i;
    char edgeUndir;
    edgeUndir = 0;
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
    }
    return 1-edgeUndir;
}

char URF_checkGraphCorrect(URF_graph *gra)
{
    /*int i,j;
    int from, to;*/
    char edgeUndir;
    char connected;
    connected = 0;
    edgeUndir = 0;
    
    /*check for directed edges is commented out because currently with the way the edges are added, the graph is always undirected*/
    edgeUndir = 1-URF_checkGraphDirected(gra);
    
    connected = URF_checkGraphConnected(gra);

    if(edgeUndir == 0 || connected == 0)
    {
        return 0;
    }
    return 1;
}

int URF_isAdj(URF_graph *graph, int i, int j)
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

void URF_deleteGraph(URF_graph *gra)
{
    int i;
    assert(gra != NULL);
    if(URF_checkGraphDirected(gra) == 0)/*undirected*/
    {
        for(i=0; i<gra->E; ++i)
        {
            free(gra->edges[i]);
        }
    }
    free(gra->edges);
    for(i=0; i<gra->V; ++i)
    {
        if(gra->degree[i] > 0)
        {
            free(gra->adjList[i]);
        }
    }
    free(gra->adjList);
    free(gra->degree);
    free(gra);
}

void URF_printGraph(URF_graph *graph)
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
    if(URF_checkGraphDirected(graph) == 0)/*undirected*/
    {
        printf("edges:\n");
        for(i=0; i<graph->E; ++i)
        {
            printf("%d: [%d,%d]\n", i, graph->edges[i][0], graph->edges[i][1]);
        }
    }
}

URF_graph *initNewGraph(int V)
{
    URF_graph *graph = malloc(sizeof(*graph));
    int *degree;
    int i;
    degree = malloc(V * sizeof(*degree));
    for(i=0; i<V; ++i)
    {
        degree[i] = 0;
    }
    URF_initGraph(graph,V,degree);
    return graph;
}

void URF_addEdge(URF_graph *gra, int from, int to)
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

void URF_addToEdgeArray(URF_graph *gra, int from, int to)
{
    int temp;
    if(from > to)
    {
        temp = from;
        from = to;
        to = temp;
    }
    if(gra->E == gra->edgesAlloced)
    {
        gra->edgesAlloced *= 2;
        gra->edges = realloc(gra->edges, gra->edgesAlloced * sizeof(*gra->edges));
    }
    gra->edges[gra->E-1] = malloc(2 * sizeof(**gra->edges));
    gra->edges[gra->E-1][0] = from;
    gra->edges[gra->E-1][1] = to;
}

void addUEdge(URF_graph *gra, int from, int to)
{
    int i;
    for(i=0; i<gra->degree[from]; ++i)
    {
        if(gra->adjList[from][i] == to)
        {/*edge already exists*/
            return;
        }
    }
    URF_addEdge(gra, from, to);
    URF_addEdge(gra, to, from);
    --gra->E; /*was incremented twice*/

    URF_addToEdgeArray(gra, from, to);
}

int URF_edgeId(URF_graph *gra, int from, int to)
{
    int edge;
    char found;
    
    found = 'f';
    if(from > to)
    {/*URF_swap order to make from < to*/
        edge = to;
        to = from;
        from = edge;
    }

    for(edge=0; edge<gra->E; ++edge)
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
        URF_warn('e');
    }
    return edge;
}
