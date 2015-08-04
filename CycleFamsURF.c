/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "apsp.h"
#include "graphURF.h"
#include "CycleFamsURF.h"
#include "CFquicksort.h"

/** returns 1 if the intersection of P(r,y) and P(r,z) is equal to {r}; 0 otherwise */
int pathsShareOnlyStart(int r, int y, int z, GraphURF *gra, sPathInfo *spi)
{
    int result = 0, i, pnt, count=0;
    int *vertInRY, *vertInRZ; /*edges in P(r,y) and P(r,z)*/
    vertInRY = malloc(gra->V * sizeof(*vertInRY));
    vertInRZ = malloc(gra->V * sizeof(*vertInRZ));
    
    for(i=0; i<gra->V; ++i)
    {
        vertInRY[i] = 0;
        vertInRZ[i] = 0;
    }
    vertInRY[y] = 1;
    vertInRZ[z] = 1;
    /*find out which vertices are on the path P(r,y)*/
    pnt = y;
    do
    {
      pnt = spi->pred[r][pnt];  
      vertInRY[pnt] = 1;
    }while(pnt != r);
    /*find out which vertices are on the path P(r,z)*/
    pnt = z;
    do
    {
      pnt = spi->pred[r][pnt];  
      vertInRZ[pnt] = 1;
    }while(pnt != r);
    /*find out if more than one vertex is shared by the paths*/
    for(i=0; i<gra->V && count<2; ++i)
    {
        if(vertInRY[i] == 1 && vertInRZ[i] == 1)
        {
            ++count;
        }
    }
    /*if r is the only shared vertex of the two paths return 1*/
    if(count == 1 && (vertInRY[r] == 1) && (vertInRZ[r] == 1))
    {
        result = 1;
    }
    
    free(vertInRY);
    free(vertInRZ);
    return result;
}

/** returns the id-no of the edge (from,to) [used in findPrototype]*/
int giveEdgeNo(int from, int to, GraphURF* gra)
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
        if(gra->edges[edge][0] == from && gra->edges[edge][1] == to)
        {
            break;
        }
    }
    return edge;
}

/** returns a cycle vector (element of {0,1}^m). odd (x=INT_MAX) or even cycle.*/
char *findPrototype(int r, int y, int z, int x, GraphURF *gra, sPathInfo *spi)
{
    int i, vert1, vert2;
    char *proto;
    
    proto = malloc(gra->E * sizeof(*proto));
    for(i=0; i<gra->E; ++i)
    {
        proto[i] = 0;
    }
    /*path from r to y*/
    vert1 = y;
    do
    {
        vert2 = vert1;
        vert1 = spi->pred[r][vert1];
        proto[giveEdgeNo(vert1, vert2, gra)] = 1;
    }while(vert1 != r);
    /*path from r to z*/
    vert1 = z;
    do
    {
        vert2 = vert1;
        vert1 = spi->pred[r][vert1];
        proto[giveEdgeNo(vert1, vert2, gra)] = 1;
    }while(vert1 != r);
    if(x == INT_MAX)/*odd cycle*/
    {
        proto[giveEdgeNo(y,z,gra)] = 1;
    }
    else /*even cycle*/
    {
        proto[giveEdgeNo(y,x,gra)] = 1;
        proto[giveEdgeNo(z,x,gra)] = 1;
    }
    return proto;
}

/** fills the rc datastructure with the odd cycle r-y-z-r */
void addOdd(int r, int y, int z, GraphURF *gra, sPathInfo *spi, cfURF *rc)
{
    cfam *new = malloc(sizeof(*new));
    new->r = r;
    new->p = y;
    new->q = z;
    new->x = INT_MAX; //odd cycle
    new->mark = 0;
    new->prototype = findPrototype(r, y, z, INT_MAX, gra, spi);
    new->weight = spi->dist[r][y] + spi->dist[r][z] + 1;
    rc->fams[rc->nofFams++] = new;
}

/** fills the rc datastructure with the even cycle r-y-x-z-r */
void addEven(int r, int y, int x, int z, GraphURF *gra, sPathInfo *spi, cfURF *rc)
{
    cfam *new = malloc(sizeof(**rc->fams));
    new->r = r;
    new->p = y;
    new->q = z;
    new->x = x; //even cycle
    new->mark = 0;
    new->prototype = findPrototype(r, y, z, x, gra, spi);
    new->weight = spi->dist[r][y] + spi->dist[r][z] + 2;
    rc->fams[rc->nofFams++] = new;
}

/** returns 1 if y is adjacent to z or 0 otherwise */
int adjacent(int y, int z, GraphURF *gra)
{
    int i, result = 0;
    for(i=0; i<gra->degree[y]; ++i)
    {
        if(gra->adjList[y][i] == z)
        {
            result = 1;
            break;
        }
    }
    return result;
}

/** finds a number of cycle families that contain at least all RELEVANT cycle families - just like in Vismara's pseudocode */
void vismara(cfURF *rc, GraphURF *gra, sPathInfo *spi)
{
    int i;
    int rv,yv,zv,pv,qv; /*variables as in Vismara's algorithm, extended by a 'v'*/
    char *evenCand; /*'S' in Vismara's algorithm*/
    evenCand = malloc(gra->V * sizeof(*evenCand));
    
    for(rv=0; rv<gra->V; ++rv)
    {
        for(yv=0; yv<gra->V; ++yv)
        {/*all yv reachable from rv respecting the ordering*/
            if(spi->reachable[rv][yv] == 1)
            {
                /*reset evenCand*/
                for(i=0; i<gra->V; ++i)
                {
                    evenCand[i] = 0;
                }
                for(zv=0; zv<gra->V; ++zv)
                {/*all zv reachable from rv respecting the ordering and adjacent to yv*/
                    if(spi->reachable[rv][zv] == 1 && adjacent(yv,zv,gra) == 1)
                    {
                        if(spi->dist[rv][zv] + 1 == spi->dist[rv][yv])
                        {
                            evenCand[zv] = 1;
                        }
                        else if(spi->dist[rv][zv] != spi->dist[rv][yv] + 1
                                && (gra->degree[zv] < gra->degree[yv] || (gra->degree[zv] == gra->degree[yv] && zv<yv))
                                && pathsShareOnlyStart(rv, yv, zv, gra, spi) == 1)
                        {/*add odd cycle rv-yv rv-zv zv-yv*/
                            addOdd(rv, yv, zv, gra, spi, rc);
                        }
                        /*to fill dPaths in sPathInfo with the edges to r*/
                        if(spi->dist[rv][zv] == 1)
                        {
                            addEdge(spi->dPaths[rv], zv, rv);
                        }
                    }
                }
                /*any pair in evenCand*/
                for(pv=0; pv<gra->V; ++pv)
                    for(qv=pv+1; qv<gra->V; ++qv)
                    {
                        if((evenCand[pv] == 1) && (evenCand[qv] == 1)
                            && (pathsShareOnlyStart(rv, pv, qv, gra, spi) == 1))
                        {/*add even cycle rv-pv rv-qv pv-yv-qv*/
                            addEven(rv, pv, yv, qv, gra, spi, rc);
                        }
                    }
                /*to fill dPaths in sPathInfo/fill U_r (see Vismara)*/
                for(pv=0; pv<gra->V; ++pv)
                {
                    if(evenCand[pv] == 1)
                    {
                        addEdge(spi->dPaths[rv], yv, pv);
                    }
                }
            }
        }
    }
    
    free(evenCand);
}

cfURF *findCycleFams(GraphURF *gra, sPathInfo *spi)
{
    cfURF *rc = malloc(sizeof(*rc));
    rc->fams = malloc((2*gra->E*gra->E + (gra->E - gra->V +1) * gra->V) * sizeof(*rc->fams)); /*number of CFs is at most 2m²+vn (Vismara Lemma 3)*/
    rc->nofFams = 0;
    vismara(rc, gra, spi);
    sortbyweight(rc);
    return rc;
}

void deleteCycleFams(cfURF *rc)
{
    int i;
    for(i=0; i<rc->nofFams; ++i)
    {
        free(rc->fams[i]->prototype);
        free(rc->fams[i]);
    }
    free(rc->fams);
    free(rc);
}

