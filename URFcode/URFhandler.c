/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "apsp.h"
#include "URF_graph.h"
#include "URFhandler.h"

void giveVertices(int a, int b, char *array, URF_graph *gra, sPathInfo *spi)
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

/*int edgeIdx(int from, int to, URF_graph *gra)
{
    int edge;
    
    if(from > to)
    {//swap order to make from < to
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
}*/

void giveEdges(int a, int b, char *array, URF_graph *gra, sPathInfo *spi)
{
/* similar to the function "List_Paths" from Vismara*/
    int i, vertex;
    
    if(a==b) return;
    
    for(i=0; i<spi->dPaths[a]->degree[b]; ++i)/*each vertex adjacent to b in U_a*/
    {
        vertex = spi->dPaths[a]->adjList[b][i];
        array[edgeId(gra,b,vertex)] = 1;
        giveEdges(a, vertex, array, gra, spi);
    }
}

/** just like the function "List_Paths" by Vismara: finds all shortest paths between r and p recursively and writes them into the array "paths". 'mode' determines if the paths are stored as arrays of vertices or of edges.*/
void listPaths(int r, int p, char ***paths, int *currPath, int *alloced, char mode, URF_graph *gra, sPathInfo *spi)
{
    int number; /*number of edges or of vertices in the graph*/
    int vertex;
    int i,j,k;
    char *copyBeforeSplit;
    
    if(mode == 'a') number = gra->V;
    else number = gra->E;
    
    if(mode == 'a')
    {
        (*paths)[*currPath][p] = 1;
    }
    if(r==p)
    {
        return;
    }
    copyBeforeSplit = malloc(number * sizeof(*copyBeforeSplit));
    
    if(spi->dPaths[r]->degree[p] > 1)/*split*/
    {
        for(i=0; i<number; ++i)/*make a copy of the path so far*/
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
                        (*paths)[j] = malloc(number * sizeof(*(*paths)[j]));
                        for(k=0; k<number; ++k)
                        {
                            (*paths)[j][k] = 0;
                        }
                    }
                }
                for(i=0; i<number; ++i)/*get the path up until the split*/
                {
                    (*paths)[*currPath][i] = copyBeforeSplit[i];
                }
            }
            if(mode == 'b')
            {
                (*paths)[*currPath][edgeId(gra,p,vertex)] = 1;
            }
            listPaths(r,vertex,paths,currPath,alloced,mode,gra,spi);
        }
    }
    else/*not a split*/
    {
        vertex = spi->dPaths[r]->adjList[p][0];
        if(mode == 'b')
        {
            (*paths)[*currPath][edgeId(gra,p,vertex)] = 1;
        }
        listPaths(r,vertex,paths,currPath,alloced,mode,gra,spi);
    }
    free(copyBeforeSplit);
}
    
void getPaths(int r, int p, char ***paths, int alloced, char mode, URF_graph *gra, sPathInfo *spi)
{
    int i,j;
    int currPath=0;
    int number;
    
    *paths = malloc(alloced * sizeof(**paths));
    for(i=0; i<alloced; ++i)
    {
        if(mode == 'a')
        {
            number = gra->V;
        }
        else
        {
            number = gra->E;
        }
        (*paths)[i] = malloc(number * sizeof(*((*paths)[i])));
        for(j=0; j<number; ++j)
        {
            (*paths)[i][j] = 0;
        }
    }
    listPaths(r,p,paths,&currPath,&alloced,mode,gra,spi);
    if(currPath+1 == alloced)
    {
        *paths = realloc(*paths, (alloced+1)*sizeof(**paths));
    }
    else
    {
        for(i=currPath+1; i<alloced; ++i)
        {
            free((*paths)[i]); /*to avoid memory leak*/
        }
    }
    (*paths)[currPath+1] = NULL;
}

int combinePaths(char ***paths1, char ***paths2, int p, int q, int x, char ***result, int currIdx, int alloced, char mode, URF_graph *gra)
{
    char *path1, *path2;
    int i,j,k;
    char *cycle;
    int number;
    
    number = (mode == 'a') ? gra->V : gra->E;
    path1=(*paths1)[0];
    for(j=0; path1!=NULL; ++j)
    {
        path2=(*paths2)[0];
        for(k=0; path2!=NULL; ++k)
        {/*all combinations of paths*/
            cycle=malloc(number * sizeof(*cycle));
            for(i=0; i<number; ++i)
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
            if(mode == 'a')
            {
                if(x < INT_MAX)/*even cycle*/
                {
                    cycle[x] = 1;
                }
            }
            else /*mode == 'b'*/
            {
                if(x < INT_MAX)/*even cycle*/
                {
                    cycle[edgeId(gra,p,x)] = 1;
                    cycle[edgeId(gra,q,x)] = 1;
                }
                else /*odd cycle*/
                {
                    cycle[edgeId(gra,p,q)] = 1;
                }
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
    for(j=0; (*paths1)[j]!=NULL; ++j)
    {
        free((*paths1)[j]);
    }
    for(j=0; (*paths2)[j]!=NULL; ++j)
    {
        free((*paths2)[j]);
    }
    free(*paths1);
    free(*paths2);
    return currIdx;
}

