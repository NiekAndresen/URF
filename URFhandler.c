/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

/** just like the function "List_Paths" by Vismara: finds all shortest paths between r and p recursively and writes them into the array "paths".*/
void listPaths(int r, int p, char ***paths, int *currPath, int *alloced, GraphURF *gra, sPathInfo *spi)
{
    int vertex;
    int i,j,k;
    char *copyBeforeSplit;
    
    (*paths)[*currPath][p] = 1;
    if(r==p)
    {
        return;
    }
    copyBeforeSplit = malloc(gra->V * sizeof(*copyBeforeSplit));
    
    if(spi->dPaths[r]->degree[p] > 1)/*split*/
    {
        for(i=0; i<gra->V; ++i)/*make a copy of the path so far*/
        {
            copyBeforeSplit[i] = (*paths)[*currPath][i];
        }
        for(i=0; i<spi->dPaths[r]->degree[p]; ++i)/*each vertex adjacent to p in U_r*/
        {
            vertex = spi->dPaths[r]->adjList[p][i];
            if(i!=0)/*not continuing the last path*/
            {
                *currPath += 1;
                if(*currPath == *alloced)/*more space needed*/
                {
                    *paths = realloc(*paths, 2*(*alloced)*sizeof(**paths));
                    *alloced *= 2;
                    for(j=*currPath; j<*alloced; ++j)
                    {
                        (*paths)[j] = malloc(gra->V * sizeof(*(*paths)[j]));
                        for(k=0; k<gra->V; ++k)
                        {
                            (*paths)[j][k] = 0;
                        }
                    }
                }
                for(i=0; i<gra->V; ++i)/*get the path up until the split*/
                {
                    (*paths)[*currPath][i] = copyBeforeSplit[i];
                }
            }
            listPaths(r,vertex,paths,currPath,alloced,gra,spi);
        }
    }
    else/*not a split*/
    {
        vertex = spi->dPaths[r]->adjList[p][0];
        listPaths(r,vertex,paths,currPath,alloced,gra,spi);
    }
    free(copyBeforeSplit);
}
    
void getPaths(int r, int p, char ***paths, int alloced, GraphURF *gra, sPathInfo *spi)
{
    int i,j;
    int currPath=0;
    *paths = malloc(alloced * sizeof(**paths));
    for(i=0; i<alloced; ++i)
    {
        (*paths)[i] = malloc(gra->V * sizeof(*(*paths)[i]));
        for(j=0; j<gra->V; ++j)
        {
            (*paths)[i][j] = 0;
        }
    }
    listPaths(r,p,paths,&currPath,&alloced,gra,spi);
    if(currPath+1 == alloced)
    {
        *paths = realloc(*paths, (alloced+1)*sizeof(**paths));
    }
    else
    {
        free((*paths)[currPath+1]); /*to avoid memory leak*/
    }
    (*paths)[currPath+1] = NULL;
}

int combinePaths(char ***paths1, char ***paths2, int x, char ***result, int currIdx, int alloced, GraphURF *gra)
{
    char *path1, *path2;
    int i,j,k;
    char *cycle;
    
    path1=(*paths1)[0];
    for(j=0; path1!=NULL; ++j)
    {
        path2=(*paths2)[0];
        for(k=0; path2!=NULL; ++k)
        {/*all combinations of paths*/
            cycle=malloc(gra->V * sizeof(*cycle));
            for(i=0; i<gra->V; ++i)
            {
                if(path1[i]==1 || path2[i]==1)
                {
                    cycle[i] = 1;
                }
                else
                {
                    cycle[i] = 0;
                }
            }
            if(x < INT_MAX)/*even cycle*/
            {
                cycle[x] = 1;
            }
            if(currIdx == alloced)/*more space needed*/
            {
                *result = realloc(*result, 2*alloced*sizeof(**result));
                alloced *= 2;
            }
            (*result)[currIdx++] = cycle;
            path2=(*paths2)[k+1];
        }
        path1=(*paths1)[j+1];
    }
    /*delete paths*/
    path1=(*paths1)[0];
    for(j=0; path1!=NULL; ++j)
    {
        free(path1);
        path1=(*paths1)[j+1];
    }
    path2=(*paths2)[0];
    for(j=0; path2!=NULL; ++j)
    {
        free(path2);
        path2=(*paths2)[j+1];
    }
    free(*paths1);
    free(*paths2);
    return currIdx;
}

