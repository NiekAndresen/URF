/** Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "graph.h"
#include "utility.h"
#include "apsp.h"

void initializeSPathInfo(sPathInfo *info, Graph *gra)
{
    info->pred = alloc2DIntArray(gra->V, gra->V);
    info->dist = alloc2DIntArray(gra->V, gra->V);
    info->reachable = alloc2DCharArray(gra->V, gra->V);
}

void findpaths(sPathInfo *spi, Graph *gra)
{
    int i,j,w,adj,run;
    int q_head, q_nextfree, q_size;
    char *color = malloc(gra->V * sizeof(*color));
    int *queue = malloc(gra->V * sizeof(*queue));
    
    for(run=1; run<3; ++run) //2 run throughs to get Vr correct
    {
        for(i=0; i<gra->V; ++i)
        {
            //initialization
            q_head=0;
            q_nextfree=0;
            q_size=0;
            for(j=0; j<gra->V; ++j)
            {
                if(run==1)
                {
                    spi->dist[i][j] = INT_MAX; //infinity
                    spi->pred[i][j] = INT_MAX;
                    spi->reachable[i][j] = 0;
                }
                color[j] = 'w'; //white
            }
            spi->dist[i][i] = 0;
            color[i] = 'b'; //black
            queue[q_nextfree]=i; //enqueue
            ++q_nextfree; //enqueue
            ++q_size; //enqueue
            
            while(q_size > 0)
            {
                w = queue[q_head]; //dequeue
                ++q_head; //dequeue
                --q_size; //dequeue
                for(adj=0; adj<gra->degree[w]; ++adj) //for each node adj to w
                {
                    j=gra->adjList[w][adj];
                    if(run==2 || ((gra->degree[j] == gra->degree[i]) && j<i) || gra->degree[j] < gra->degree[i]) //j precedes i in order (only first run)
                    if(color[j] == 'w') //unvisited
                    {
                        if(run==2)
                        {//if in the 2nd run a dist gets shorter
                            if(spi->dist[i][w]+1 < spi->dist[i][j])
                            {//not element of Vr
                                spi->reachable[i][j] = 0;
                            }
                        }
                        spi->dist[i][j] = spi->dist[i][w] + 1;
                        spi->pred[i][j] = w; //predecessor of j on a shortest path from i to j
                        color[j] = 'b';
                        queue[q_nextfree] = j; //enqueue
                        ++q_nextfree; //enqueue
                        ++q_size; //enqueue
                        if(run==1)
                        {
                            spi->reachable[i][j] = 1;
                        }
                    }
                }
            }
        }
    }
    
    free(color);
    free(queue);
}

sPathInfo *AllPairsShortestPaths(Graph *gra)
{
    sPathInfo *info = malloc(sizeof(*info));
    
    initializeSPathInfo(info, gra);
    findpaths(info, gra);
    
    return info;
}

void deleteAPSP(sPathInfo *info)
{
    delete2DArray((void **)info->pred);
    delete2DArray((void **)info->dist);
    delete2DArray((void **)info->reachable);
    free(info);
}
