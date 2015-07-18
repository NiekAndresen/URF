/* Niek Andresen 2015 - Bachelor Thesis */

#include <limits.h>
#include "apsp.h"
#include "graphURF.h"
#include "relevantCyclesURF.h"

/** returns 1 if the intersection of P(r,y) and P(r,z) is equal to {r}; 0 otherwise */
int pathsShareOnlyStart(int r, int y, int z, GraphURF *gra, sPathInfo *spi)
{
    int result = 0, i, pnt, count=0;
    int *edgesInRY, *edgesInRZ; /*edges in P(r,y) and P(r,z)*/
    vertInRY = malloc(gra->V * sizeof(*vertInRY));
    vertnRZ = malloc(gra->V * sizeof(*vertInRZ));
    
    for(i=0; i<gra->V; ++i)
    {
        vertInRY[i] = 0;
        vertInRZ[i] = 0;
    }
    vertInRY[y] = 1;
    vertInRZ[z] = 1;
    /*find out which vertices are on the path P(r,y)*/
    pnt = spi->pred[r][y]
    do
    {
      vertInRY[pnt] = 1;
      pnt = spi->pred[r][pnt];  
    }while(pnt != r)
    /*find out which vertices are on the path P(r,z)*/
    pnt = spi->pred[r][z]
    do
    {
      vertInRZ[pnt] = 1;
      pnt = spi->pred[r][pnt];  
    }while(pnt != r)
    /*find out if more than one vertex is shared by the paths*/
    for(i=0; i<gra->V, count<2; ++i)
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

void addOdd(int r, int y, int z, GraphURF *gra, sPathInfo *spi, rcURF *rc)//TODO inprogress
{
    rcf *new = malloc(sizeof(**rc->fams));
    int *proto = malloc(gra->E * sizeof(*proto));
    new->r = r;
    new->p = y;
    new->q = z;
    new->x = INT_MAX; //odd cycle
    new->mark = 0;
    //TODO function that finds edge numbers on path
    rc->prototype = proto;
    rc->fams[rc->iter++] = new;
    
}

/** just like in Vismara's pseudocode */
void vismara(rcURF *rc, GraphURF *gra, sPathInfo *spi)//TODO inprogress
{
    int i, j, famNo=0;
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
                for(zv=0; zv<gra-V; ++v)
                {/*all zv reachable from rv respecting the ordering*/
                    if(spi->reachable[rv][zv] == 1)
                    {   
                        if(spi->dist[rv][zv] + 1 = spi->dist[rv][yz])
                        {
                            evenCand[zv] = 1;
                        }
                        else if(spi->dist[rv][zv] != spi->dist[rv][yv] + 1
                                && (gra->degree[zv] < gra->degree[yv] || (gra->degree[zv] == gra->degree[yv] && zv<yv))
                                && pathsShareOnlyStart(rv, yv, zv, gra, spi) == 1)
                        {/*add odd cycle rv-yv rv-zv zv-yv*/
                            addOdd(rv, yv, zv, gra, spi, rc);
                        }
                    }
                }
                /*any pair in evenCand*/
                for(pv=0; pv<gra->V; ++pv)
                    for(qv=pv+1 qv<gra-V; ++qv)
                    {
                        if((evenCand[pv] == 1) && (evenCand[qv] == 1)
                            && (pathsShareOnlyStart(rv, pv, qv, gra, spi) == 1))
                        {/*add even cycle rv-pv rv-qv pv-yv-qv*/
                            
                        }
                    }
            }
        }
    }
    
    free(evenCand);
}

rcURF *findRelCycles(GraphURF *gra, sPathInfo *spi)
{
    rcURF *rc = malloc(sizeof(*rc));
    rc->fams = malloc(2*gra->E*gra->E + (gra->E-gra->V+1)*gra-V)); /*number of RCFs is at most 2m²+vn (Vismara Lemma 3)*/
    rc->nofFams = 0;
    rc->iter = 0;
    vismara(rc, gra, spi);
    return rc;
}

void deleteRelCycles(rcURF *rc)
{
    free(rc->fams);
    free(rc);
}

