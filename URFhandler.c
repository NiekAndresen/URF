/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include "apsp.h"
#include "graphURF.h"
#include "URFhandler.h"

void giveVertices(int a, int b, char *array, GraphURF *gra, sPathInfo *spi)
{
/* similar to the function "List_Paths" from Vismara*/
    int i, vertex;
    
    if(a==b)
    {
        array[a] = 1;
        return;
    }
    array[b] = 1;
    for(i=0; i<spi->dPaths[a]->degree[b]; ++i)/*each vertex adjacent to b in U_a*/
    {
        vertex = spi->dPaths[a]->adjList[b][i];
        giveVertices(a, vertex, array, gra, spi);
    }
}

int edgeIdx(int from, int to, GraphURF *gra)
{
    int edge;
    
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
            break;
        }
    }
    return edge;
}

void giveEdges(int a, int b, char *array, GraphURF *gra, sPathInfo *spi)
{
/* similar to the function "List_Paths" from Vismara*/
    int i, vertex;
    
    if(a==b) return;
    
    for(i=0; i<spi->dPaths[a]->degree[b]; ++i)/*each vertex adjacent to b in U_a*/
    {
        vertex = spi->dPaths[a]->adjList[b][i];
        array[edgeIdx(b,vertex,gra)] = 1;
        giveEdges(a, vertex, array, gra, spi);
    }
}

