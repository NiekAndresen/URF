/* Niek Andresen 2015 - Bachelor Thesis */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "graphURF.h"
#include "utility.h"
#include "URF.h"

int main(int argc, char **argv)
{
    GraphURF *graph;
    int i,j;
    
    graph = readInList("adjlist");
    printGraph(graph);
    
    urfdata *udata = calculateURFs(graph);
    
    printf("calculated\n");
    /*
    printf("dist:\n");
    print2DIntArray(udata->spi->dist, graph->V, graph->V);
    printf("pred:\n");
    print2DIntArray(udata->spi->pred, graph->V, graph->V);
    printf("reachable:\n");
    print2DCharArray(udata->spi->reachable, graph->V, graph->V);
    printf("dPaths:\n");
    for(i=0; i<graph->V; ++i)
    {
        printf("r=%d\n",i);
        printGraph(udata->spi->dPaths[i]);
    }
    */  
    printf("CFs:\n");
    printf("Number of Fams: %d\n", udata->CFs->nofFams);
    for(i=0; i<udata->CFs->nofFams; ++i)
    {
        if(udata->CFs->fams[i]->x == INT_MAX)
        {
            printf("odd: r=%d, p=%d, q=%d\n",udata->CFs->fams[i]->r,udata->CFs->fams[i]->p,udata->CFs->fams[i]->q);
        }
        else
        {
            printf("even: r=%d, p=%d, q=%d, x=%d\n",udata->CFs->fams[i]->r,udata->CFs->fams[i]->p,udata->CFs->fams[i]->q,udata->CFs->fams[i]->x);
        }
        printf("a prototype: ");
        for(j=0; j<graph->E; ++j)
        {
            printf(" %d", udata->CFs->fams[i]->prototype[j]);
        }
        printf("\n");
    }
    /*
    printf("mark:\n");
    for(i=0; i<udata->CFs->nofFams; ++i)
    {
        printf("  %d\n",udata->CFs->fams[i]->mark);
    }
    */
    printf("\nURFs:\n");
    
    /*printf("\nURFrel:\n");
    int k;
    for(i=0; i<udata->urfInfo->nofWeights; ++i)
    {
        printf("weight %d\n",i);
        for(j=0; j<udata->urfInfo->nofProtos[i]; ++j)
        {
            for(k=0; k<udata->urfInfo->nofProtos[i]; ++k)
            {
                printf("%d ",udata->urfInfo->URFrel[i][j][k]);
            }
            printf("\n");
        }
    }*/
    
    printf("number of URFs: %d\n",udata->nofURFs);
    for(i=0; i<udata->nofURFs; ++i)
    {
        printf("number of CFs in URF %d: %d\n",i,udata->urfInfo->nofCFsPerURF[i]);
        printf("these are:");
        for(j=0; j<udata->urfInfo->nofCFsPerURF[i]; ++j)
        {
            printf(" (r:%d,p:%d,q:%d,x:%d) ",udata->urfInfo->URFs[i][j]->r,udata->urfInfo->URFs[i][j]->p,udata->urfInfo->URFs[i][j]->q,udata->urfInfo->URFs[i][j]->x);
        }
        printf("\n");

        /*int l,number;
        char **cycles;
        char *k;
        char mode = 'a';
        number = (mode=='a')?graph->V:graph->E;
        printf("cycles:");
        cycles = giveURFCycles(udata, i, mode);
        k=cycles[0];
        for(j=0; k!=NULL; ++j)
        {
            printf("\n");
            for(l=0; l<number; ++l)
            {
                printf("%d ",k[l]);
            }
            k=cycles[j+1];
        }
        printf("\n");
        deleteCycles(cycles);*/
        
        /*int **URFedges;
        URFedges = giveURFBonds(udata, i);
        printf("edges of URF %d:\n",i);
        for(j=0; URFedges[j]!=NULL; ++j)
        {
            printf("%d: [%d,%d]\n", j, URFedges[j][0], URFedges[j][1]);
        }
        deleteBondArr(URFedges);*/
    }
    
    int *URFatoms;
    printf("atoms in the URFs:\n");
    for(i=0; i<udata->nofURFs; ++i)
    {
        URFatoms = giveURFAtoms(udata, i);
        for(j=0; j<graph->V; ++j)
        {
            printf("%d ",URFatoms[j]);
        }
        printf("\n");
        free(URFatoms);
    }
    
    /*
    char **basis = findBasis(udata);
    printf("basis:\n");
    for(i=0; i<graph->E-graph->V+1; ++i)
    {
        for(j=0; j<graph->V;  ++j)
        {
            printf("%d ",basis[i][j]);
        }
        printf("\n");
    }
    deleteArr(basis);*/
    
    /*int atom;
    int *URFsWithAtom;
    atom=1;
    URFsWithAtom = listURFs(udata, atom, 'a');
    printf("listing all URFs with atom %d: ",atom);
    for(i=0; URFsWithAtom[i]<INT_MAX; ++i)
    {
        printf("%d ",URFsWithAtom[i]);
    }
    printf("\n");
    printf("number of URFs with atom %d: %d\n",atom,numOfURFsContaining(udata, atom));
    free(URFsWithAtom);*/
    
    /*char **protos;
    printf("RC prototypes:\n");
    protos=giveRCprototypes(udata);
    for(i=0; protos[i]!=NULL; ++i)
    {
        for(j=0; j<graph->V; ++j)
        {
            printf("%d ",protos[i][j]);
        }
        printf("\n");
    }
    deleteArr(protos);*/
    
    /*char **relCycles;
    printf("RC prototypes:\n");
    relCycles=giveRCcycles(udata);
    for(i=0; relCycles[i]!=NULL; ++i)
    {
        for(j=0; j<graph->V; ++j)
        {
            printf("%d ",relCycles[i][j]);
        }
        printf("\n");
    }
    deleteCycles(relCycles);*/
    
    
    
    deleteURFdata(udata);
    printf("Niek ist die Nummer %d\n",1);
    return EXIT_SUCCESS;
}
