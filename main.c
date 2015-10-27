/* Niek Andresen 2015 - Bachelor Thesis */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./io.h"
#include "./URFcode/URF_graph.h"
#include "./URFcode/utility.h"
#include "URF.h"
/** returns the index in the array of RCFs (fams) that the RCF with the weight
at the index "weight" and position j has */
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
    URF_graph *graph;
    int i,j,k;
    URF_data *udata;
    time_t time;
    
    if(argc == 1)
    {
        graph = readInList("adjlist");
    }
    else
    {
        graph = readInList(argv[1]);
    }
    
    URF_printGraph(graph);

    time = clock();
    udata = URF_calculate(graph);
    time = clock() - time;
    
    printf("calculated, it took %fs\n",(float)time/CLOCKS_PER_SEC);
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
    if(udata->nofURFs>0)
    {
        printf("\nURFs:\n");
        
        printf("\nURFrel:\n");
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
        number = URF_getCycles(udata, &cycles, i);
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
        URF_deleteCycles(cycles, number);*/
        
        URFBond *URFedges;
        int number4;
        number4 = URF_getBonds(udata, i,&URFedges);
        printf("edges of URF %d:\n",i);
        for(j=0; j<number4; ++j)
        {
            printf("%d: [%d,%d]\n", j, URFedges[j][0], URFedges[j][1]);
        }
        free(URFedges);
    }
    
    URFAtom *URFatoms;
    int number3;
    printf("atoms in the URFs:\n");
    for(i=0; i<udata->nofURFs; ++i)
    {
        number3 = URF_getAtoms(udata, i, &URFatoms);
        j=0;
        for(k=0; k<graph->V; ++k)
        {
            if(j<number3 && URFatoms[j] == k)
            {
                printf("1 ");
                ++j;
            }
            else
            {
                printf("0 ");
            }
        }
        printf("\n");
        free(URFatoms);
    }
    
    URFCycle *basis;
    int number1;
    number1 = URF_getBasis(udata, &basis);
    printf("basis:\n");
    for(i=0; i<number1; ++i)
    {
        for(j=0; j<basis[i].weight; ++j)
        {
            printf("(%d ",basis[i].bonds[j][0]);
            printf("%d), ",basis[i].bonds[j][1]);
        }
        printf("\n");
    }

    printf("or:\n");
    char **basis2;
    number1 = URF_translateCycArray(udata, basis, number1, &basis2);
    for(i=0; i<number1; ++i)
    {
        for(j=0; j<udata->graph->E; ++j)
        {
            printf("%d ",basis2[i][j]);
        }
        printf("\n");
    }
    URF_deleteEdgeIdxArray(basis2, number1);
    URF_deleteCycles(basis, number1);
    
    
    /*URFAtom atom;
    int number2,*URFsWithAtom;
    atom=1;
    number2 = URF_listURFsWithAtom(udata, &URFsWithAtom, atom);
    printf("listing all URFs with atom %d: ",atom);
    for(i=0; i<number2; ++i)
    {
        printf("%d ",URFsWithAtom[i]);
    }
    printf("\n");
    printf("number of URFs with atom %d: %d\n",atom,URF_numContaining(udata, atom));
    free(URFsWithAtom);*/
    
    /*int bond,numberB;
    int *URFsWithBond;
    bond=1;
    numberB = URF_listURFsWithBond(udata, &URFsWithBond, udata->graph->edges[bond][0], udata->graph->edges[bond][1]);
    printf("listing all URFs with bond %d: ",bond);
    for(i=0; i<numberB; ++i)
    {
        printf("%d ",URFsWithBond[i]);
    }
    printf("\n");
    free(URFsWithBond);*/
    
    /*URFCycle *protos;
    int num;
    printf("RC prototypes:\n");
    num = URF_getRCPrototypes(udata, &protos);
    for(i=0; i<num ; ++i)
    {
        for(j=0; j<protos[i].weight; ++j)
        {
            printf("(%d %d), ",protos[i].bonds[j][0],protos[i].bonds[j][1]);
        }
        printf("\n");
    }
    URF_deleteCycles(protos, num);*/
    
    /*URFCycle *relCycles;
    int num1;
    printf("RCs:\n");
    num1=URF_getRCycles(udata, &relCycles);
    for(i=0; i<num1; ++i)
    {
        printf("%d:  ",i+1);
        for(j=0; j<relCycles[i].weight; ++j)
        {
            printf("(%d %d)  ",relCycles[i].bonds[j][0],relCycles[i].bonds[j][1]);
        }
        printf("\n");
    }
    URF_deleteCycles(relCycles, num1);*/
    
    URF_deleteData(udata);
    /*printf("Niek ist die Nummer %d\n",1);*/
    return EXIT_SUCCESS;
}
