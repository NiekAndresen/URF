/* Niek Andresen 2015 - Bachelor Thesis */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "./URFcode/io.h"
#include "./URFcode/graphURF.h"
#include "./URFcode/utility.h"
#include "URF.h"
/** returns the index in the array of RCFs (fams) that the RCF with the weight at the index "weight" and position j has */
int idxWeightsMain(URFinfo *uInfo, int weight, int j)
{
    int i, sum = 0;
    for(i=0; i<weight; ++i)
    {
        sum += uInfo->nofProtos[i];
    }
    return sum + j;
}

int main(int argc, char **argv)
{
    GraphURF *graph;
    int i,j;
    
    if(argc == 1)
    {
        graph = readInList("adjlist");
    }
    else
    {
        graph = readInList(argv[1]);
    }
    
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
    
    printf("\nURFrel:\n");
    int k;
    for(i=0; i<udata->urfInfo->nofWeights; ++i)
    {
        printf("weight %d (index %d)\n",udata->CFs->fams[idxWeightsMain(udata->urfInfo,i,0)]->weight,i);
        for(j=0; j<udata->urfInfo->nofProtos[i]; ++j)
        {
            for(k=0; k<udata->urfInfo->nofProtos[i]; ++k)
            {
                printf("%d ",udata->urfInfo->URFrel[i][j][k]);
            }
            printf("\n");
        }
    }
    
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
        int ***cycles;
        number = giveURFCycles(udata, &cycles, i);
        printf("cycles (%d):\n",number);
        for(j=0; j<number; ++j)
        {
            for(l=0; cycles[j][l]!=NULL; ++l)
            {
                printf("(%d ",cycles[j][l][0]);
                printf("%d), ",cycles[j][l][1]);
            }
            printf("\n");
        }
        printf("\n");
        deleteCycles(cycles, number);*/
        
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
    
    
    int ***basis;
    int number;
    number = findBasis(udata, &basis);
    printf("basis:\n");
    for(i=0; i<number; ++i)
    {
        for(j=0; basis[i][j]!=NULL;  ++j)
        {
            printf("(%d ",basis[i][j][0]);
            printf("%d), ",basis[i][j][1]);
        }
        printf("\n");
    }
    deleteCycles(basis, number);
    
    /*int atom,number;
    int *URFsWithAtom;
    atom=1;
    number = listURFsWithAtom(udata, &URFsWithAtom, atom);
    printf("listing all URFs with atom %d: ",atom);
    for(i=0; i<number; ++i)
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
    
    /*int *URFatoms;
    printf("I've got the cycles:\n");
    for(i=0; i<udata->nofURFs; ++i)
    {
        URFatoms = giveURFAtoms(udata, i);
        for(j=0; j<graph->V; ++j)
        {
            if(URFatoms[j] == 1)
            {
                printf("%d ",j);
            }
        }
        printf("\n");
        free(URFatoms);
    }
    
    printf("path from 139 to 54:\n");
    printf("dist = %d\n",udata->spi->dist[139][54]);
    j=54;
    while(j!=139)
    {
        printf("%d ",j);
        j = udata->spi->pred[139][j];
    }
    printf("\n");
    
    j=64;
    printf("path from 139 to %d:\n",j);
    printf("dist = %d\n",udata->spi->dist[139][j]);
    while(j!=139)
    {
        printf("%d ",j);
        j = udata->spi->pred[139][j];
    }
    printf("\n");
    
    j=114;
    printf("path from 139 to %d:\n",j);
    printf("dist = %d\n",udata->spi->dist[139][j]);
    while(j!=139)
    {
        printf("%d ",j);
        j = udata->spi->pred[139][j];
    }
    printf("\n");
    
    j=24;
    printf("path from 139 to %d:\n",j);
    printf("dist = %d\n",udata->spi->dist[139][j]);
    while(j!=139)
    {
        printf("%d ",j);
        j = udata->spi->pred[139][j];
    }
    printf("\n");*/
    
    /*printf("dPaths[139]:\n");
    printGraph(udata->spi->dPaths[139]);*/
    
    deleteURFdata(udata);
    printf("Niek ist die Nummer %d\n",1);
    return EXIT_SUCCESS;
}
