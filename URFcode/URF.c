/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../URF.h"
#include "apsp.h"
#include "CycleFamsURF.h"
#include "URFhandler.h"
#include "utility.h"
#include "graphURF.h"
#include "URFrelation.h"

GraphURF *initNewURFGraph(int V)
{
    return initNewGraph(V);
}

void addUEdgeURF(GraphURF *gra, int from, int to)
{
    addUEdge(gra, from, to);
}

void findShortestPaths(urfdata *udata, GraphURF *gra)
{
    udata->spi = AllPairsShortestPaths(gra);
}

urfdata *calculateURFs(GraphURF *gra)
{
    char correctGraph;
    urfdata *udata;
    correctGraph = checkGraphCorrect(gra); /*from graphURF.h*/
    if(correctGraph == 0) usage('c');
    udata = malloc(sizeof(*udata));
    findShortestPaths(udata, gra); /*from apsp.h*/
    udata->CFs = findCycleFams(gra, udata->spi); /*from CycleFamsURF.h*/
    udata->graph = gra;
    if(udata->CFs->nofFams > 0)
    {
        udata->urfInfo = checkURFRelation(udata->CFs, udata->graph, udata->spi);/*from URFInfo.h*/
        udata->nofURFs = udata->urfInfo->nofURFs;
    }
    else
    {
        udata->nofURFs = 0;
    }
    return udata;
}

void deleteURFdata(urfdata *udata)
{
    deleteAPSP(udata->spi, udata->graph->V);
    deleteCycleFams(udata->CFs);
    if(udata->nofURFs > 0)
    {
        deleteURFInfo(udata->urfInfo);
    }
    deleteGraph(udata->graph);
    free(udata);
}

int numberOfURFs(urfdata *udata)
{
    return udata->nofURFs;    
}

int giveURFWeight(urfdata *udata, int index)
{
    if(udata->nofURFs < 1 || index >= udata->nofURFs) return 0;
    return udata->urfInfo->URFs[index][0]->weight;
}

/** gives an array of indices of atoms that are contained in the URF with the given index. Array is terminated by INT_MAX */
int *giveAtoms(urfdata *uData, int index)
{
    int i,nofFams,nextfree=0,alloced;
    char *atoms;
    int *result;
    cfam **URF;
    
    nofFams = uData->urfInfo->nofCFsPerURF[index];
    URF = uData->urfInfo->URFs[index];
    atoms = malloc(uData->graph->V * sizeof(*atoms));
    result = malloc(8 * sizeof(*result));
    alloced = 8;
    
    for(i=0; i<uData->graph->V; ++i)
    {
        atoms[i] = 0;
    }
    for(i=0; i<nofFams; ++i)
    {
        giveVertices(URF[i]->r, URF[i]->q, atoms, uData->graph, uData->spi);
        giveVertices(URF[i]->r, URF[i]->p, atoms, uData->graph, uData->spi);
        if(URF[i]->x < INT_MAX) /*even cycle*/
        {
            atoms[URF[i]->x] = 1;
        }
    }
    
    for(i=0; i<uData->graph->V; ++i)
    {
        if(atoms[i] == 1)
        {
            if(nextfree == alloced)
            {/*double the size*/
                result = realloc(result, 2*alloced*sizeof(*result));
                alloced *= 2;
            }
            result[nextfree++] = i;
        }
    }
    if(nextfree == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[nextfree] = INT_MAX;
    
    free(atoms);
    return result;
}

/** gives an array of indices of bonds that are contained in the URF with the given index. Array is terminated by INT_MAX */
int *giveBonds(urfdata *uData, int index)
{
    int i,nofFams,nextfree=0,alloced;
    char *bonds;
    int *result;
    cfam **URF;
    
    nofFams = uData->urfInfo->nofCFsPerURF[index];
    URF = uData->urfInfo->URFs[index];
    bonds = malloc(uData->graph->E * sizeof(*bonds));
    result = malloc(8 * sizeof(*result));
    alloced = 8;
    
    for(i=0; i<uData->graph->E; ++i)
    {
        bonds[i] = 0;
    }
    for(i=0; i<nofFams; ++i)
    {
        giveEdges(URF[i]->r, URF[i]->q, bonds, uData->graph, uData->spi);
        giveEdges(URF[i]->r, URF[i]->p, bonds, uData->graph, uData->spi);
        if(URF[i]->x < INT_MAX) /*even cycle*/
        {
            bonds[edgeId(uData->graph,URF[i]->q,URF[i]->x)] = 1;
            bonds[edgeId(uData->graph,URF[i]->p,URF[i]->x)] = 1;
        }
        else/*odd cycle*/
        {
            bonds[edgeId(uData->graph,URF[i]->q,URF[i]->p)] = 1;
        }
    }
    
    for(i=0; i<uData->graph->E; ++i)
    {
        if(bonds[i] == 1)
        {
            if(nextfree == alloced)
            {/*double the size*/
                result = realloc(result, 2*alloced*sizeof(*result));
                alloced *= 2;
            }
            result[nextfree++] = i;
        }
    }
    if(nextfree == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[nextfree] = INT_MAX;
    
    free(bonds);
    return result;
}

/** calls giveAtoms() or giveBonds() depending on mode 'a' or 'b' */
int *giveURF(urfdata *uData, int URFindex, char mode)
{
    int *result;
    if(mode == 'a')
    {
        result = giveAtoms(uData, URFindex);
    }
    else if(mode == 'b')
    {
        result = giveBonds(uData, URFindex);
    }
    else
    {
        fprintf(stderr, "ERROR: tried to call 'giveURF()' with invalid mode '%c'\n",mode);
        fprintf(stderr, "Only 'a' or 'b' are allowed. Read interface for help.\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

int giveURFAtoms(urfdata *udata, int index, int **ptr)
{
    int i;
    if(udata->nofURFs < 1 || index >=udata->nofURFs)
    {
        /*to still be able to call 'free()' on the result*/
        (*ptr) = malloc(sizeof(**ptr));
        return 0;
    }
    (*ptr) = giveAtoms(udata, index);
    for(i=0; (*ptr)[i]<INT_MAX; ++i); /*counts the number of atoms*/
    return i;
}

int giveURFBonds(urfdata *uData, int URFindex, int ***ptr)
{
    int nextfree, alloced;
    int **result;
    int *bondIndices;
    int i,j;
    if(uData->nofURFs < 1 || URFindex >=uData->nofURFs)
    {
        /*to still be able to call 'deleteBondArray()' on the result*/
        (*ptr) = malloc(sizeof(**ptr));
        (**ptr) = malloc(sizeof(***ptr));
        return 0;
    }

    nextfree = 0;
    alloced = 3;
    result = alloc2DIntArray(alloced, 2);
    bondIndices = giveBonds(uData, URFindex);
    for(i=0; bondIndices[i]<INT_MAX; ++i)
    {
        if(nextfree == alloced)/*more space needed in result*/
        {
            alloced *= 2; /* double the space */
            result = realloc(result, alloced * sizeof(*result));
            *result = realloc(*result, alloced * 2 * sizeof(**result));
            for(j=0; j<alloced; ++j)
            {
                result[j] = result[0] + 2 * j;
            }
        }
        result[nextfree][0] = uData->graph->edges[bondIndices[i]][0];
        result[nextfree][1] = uData->graph->edges[bondIndices[i]][1];
        ++nextfree;
    }
    free(bondIndices);
    (*ptr) = result;
    return nextfree;
}

/** Gives all cycles of the URF with the given index. Returns an array of cycles.
modes:
    - 'a': A cycle is represented by an array of {0,1}^n which contains a 1 at position i if the vertex i is part of the cycle or 0 otherwise (n: number of vertices).
    - 'b': A cycle is represented by an array of {0,1}^m which contains a 1 at position i if the edge i is part of the cycle or 0 otherwise (m: number of edges).
The array of cycles is ended with a terminating NULL pointer.
This structure has to be deallocated using 'deleteCyclesChar(<return value>)'.*/
char **giveURFCyclesChar(urfdata *udata, int index, char mode)
{
    int i;
    int currIdx=0; /*index of next free space in result array*/
    int alloced=2; /*space for how many cycles/paths has been allocated; is always a power of two. =2 in the beginning, grows with number of cycles*/
    int nofFams;
    cfam **URF;
    char **paths1, **paths2; /*much like the result array, but storing paths instead of cycles. paths1 for paths from r to p and paths2 for paths from r to q.*/
    char **result;
    
    nofFams = udata->urfInfo->nofCFsPerURF[index];
    URF = udata->urfInfo->URFs[index];
    
    result = malloc(alloced * sizeof(*result));
    for(i=0; i<nofFams; ++i)
    {
        getPaths(URF[i]->r, URF[i]->p, &paths1, alloced, mode, udata->graph, udata->spi);
        getPaths(URF[i]->r, URF[i]->q, &paths2, alloced, mode, udata->graph, udata->spi);
        currIdx = combinePaths(&paths1, &paths2, URF[i]->p, URF[i]->q, URF[i]->x, &result, currIdx, alloced, mode, udata->graph);
        /*calculate how many spaces are alloced with the help of the returned currIdx. Can be done since 'alloced' is always a power of two*/
        while(currIdx > alloced) alloced *= 2;
    }
    if(currIdx == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[currIdx] = NULL;
    return result;
}

/**to delete result of giveURFCyclesChar()*/
void deleteCyclesChar(char **cycles)
{
    int i;
    for(i=0; cycles[i]!=NULL; ++i)
    {
        free(cycles[i]);
    }
    free(cycles);
}

int giveURFCycles(urfdata *udata, int ****ptr, int index)
{
    char **URFCycles;
    int ***result;
    int i,j;
    unsigned int alloced, nextfree, nextBond;
    unsigned int edgeCount;
    if(udata->nofURFs < 1 || index >=udata->nofURFs) return 0;

    alloced = 4;
    nextfree = 0;
    result = malloc(alloced * sizeof(*result));
    URFCycles = giveURFCyclesChar(udata, index, 'b');
    for(i=0; URFCycles[i]!=NULL; ++i)
    {
        edgeCount = 0; /*count number of edges in this cycle*/
        for(j=0; j<udata->graph->E; ++j)
        {
            if(URFCycles[i][j] == 1)
            {
                ++edgeCount;
            }
        }
        if(nextfree == alloced) /*allocate more space*/
        {
            alloced *= 2;
            result = realloc(result, alloced * sizeof(*result));
        }
        result[nextfree] = alloc2DIntArray(edgeCount+1, 2);
        nextBond = 0;
        for(j=0; j<udata->graph->E; ++j)
        {
            if(URFCycles[i][j] == 1) /*copy edge into new structure*/
            {
                result[nextfree][nextBond][0] = udata->graph->edges[j][0];
                result[nextfree][nextBond][1] = udata->graph->edges[j][1];
                ++nextBond;
            }
        }
        result[nextfree][nextBond] = NULL;
        ++nextfree;
    }

    deleteCyclesChar(URFCycles);
    *ptr = result;
    return i;
}

void deleteCycles(int ***cycles, int number)
{
    int i;
    if(number < 1) return;
    for(i=0; i<number; ++i)
    {
        delete2DArray((void **)cycles[i]);
    }
    free(cycles);
}

/** Gives all URFs containing the object, which can be an atom or a bond.
mode:
    - 'a': atom
    - 'b': bond
returns an array of integers containing all indices of URFs containing the object.
The array ends with a terminating INT_MAX on its last position and has to be deallocated with 'free()' */
int *listURFs(urfdata *udata, int object, char mode)
{
    int *result;
    int *URFs; /*array of {0,1}^#URFs containing 1 on position i if the i-th URF contains the object*/
    int nextfree=0, alloced=2;
    int *objects;
    int i,j;
    char contained;
    
    if(!(mode == 'a' || mode == 'b'))
    {
        fprintf(stderr, "ERROR: tried to call 'listURFs()' with invalid mode '%c'\n",mode);
        fprintf(stderr, "Only 'a' or 'b' are allowed. Read interface for help.\n");
        exit(EXIT_FAILURE);
    }
    URFs = malloc(udata->nofURFs * sizeof(*URFs));
    for(i=0; i<udata->nofURFs; ++i)
    {
        objects = giveURF(udata, i, mode);
        contained = 0;
        for(j=0; objects[j]<INT_MAX; ++j)
        {
            if(objects[j] == object)
            {
                contained = 1;
                break;
            }
        }
        URFs[i] = contained;
        free(objects);
    }
    /*translate into result structure*/
    result=malloc(alloced * sizeof(*result));
    for(i=0; i<udata->nofURFs; ++i)
    {
        if(URFs[i] == 1)
        {
            if(nextfree == alloced)
            {
                result = realloc(result, 2*alloced*sizeof(*result));
                alloced *= 2;
            }
            result[nextfree++] = i;
        }
    }
    if(nextfree == alloced)
    {
        result = realloc(result, (alloced+1)*sizeof(*result));
    }
    result[nextfree] = INT_MAX;
    free(URFs);
    return result;
}

int listURFsWithAtom(urfdata *udata, int **ptr, int object)
{
    int i;
    if(udata->nofURFs < 1)
    {
        /*to still be able to call free on result*/
        (*ptr) = malloc(sizeof(**ptr));
        return 0;
    }
    *ptr = listURFs(udata, object, 'a');
    for(i=0; (*ptr)[i]<INT_MAX; ++i);
    return i;
}

int listURFsWithBond(urfdata *udata, int **ptr, int atom1, int atom2)
{
    int i;
    if(udata->nofURFs < 1)
    {
        /*to still be able to call free on result*/
        (*ptr) = malloc(sizeof(**ptr));
        return 0;
    }
    *ptr = listURFs(udata, edgeId(udata->graph, atom1, atom2), 'b');
    for(i=0; (*ptr)[i]<INT_MAX; ++i);
    return i;
}

int numOfURFsContaining(urfdata *udata, int atom)
{
    int *list = listURFs(udata, atom, 'a');
    int count;
    if(udata->nofURFs < 1) return 0;
    for(count=0; list[count]<INT_MAX; ++count);
    free(list);
    return count;
}

/** returns a set of cycles that forms a MCB of the graph.
A cycle is represented by an array of {0,1}^n with a 1 at position i if vertex i is part of the cycle. The result is an array of these cycles. The result contains |E|-|V|+1 cycles. This does not return a correct basis on an unconnected graph.*/
/*alternatives: return array of vertex indices; give option to call function only on the graph.*/
/*TODO currently not needed*/
char **findCharBasis(urfdata *udata)
{
    /*take a prototype out of each URF ordered by weight until |E|-|V|+1 cycles are collected*/
    int i,j;
    char **result;
    GraphURF *gra;
    gra = udata->graph;
    result = alloc2DCharArray(gra->E-gra->V+1, gra->V);
    for(i=0; i<gra->E-gra->V+1; ++i)
    {
        for(j=0; j<gra->V; ++j)
        {
            result[i][j] = 0;
        }
        for(j=0; j<gra->E; ++j)
        {
            if(udata->urfInfo->URFs[i][0]->prototype[j] == 1)
            {
                result[i][gra->edges[j][0]] = 1;
                result[i][gra->edges[j][1]] = 1;            
            }
        }
    }
    return result;
}

int findBasis(urfdata *udata, int ****ptr)
{
    int i,j;
    int ***result;
    char **mat; /*matrix in which the dependency checks take place*/
    int size, added;
    int currBond;
    if(udata->nofURFs < 1) return 0;

    size=udata->graph->E-udata->graph->V+1;
    mat = malloc(size * sizeof(*mat));
    result = malloc(size * sizeof(*result));
    
    added = 0;
    for(i=0; i<udata->nofURFs; ++i)
    {
        mat[added] = udata->urfInfo->URFs[i][0]->prototype;
        if(linDep(mat, added+1, udata->graph->E) == 1) /*cycles are dependent*/
        {
            /*cycle added last is not part of the basis -> will be ignored/overwritten in next step*/
            continue;
        }
        /*add cycle to result basis*/
        currBond = 0;
        result[added] = alloc2DIntArray(udata->urfInfo->URFs[i][0]->weight+1, 2);
        for(j=0; j<udata->graph->E; ++j)
        {
            if(udata->urfInfo->URFs[i][0]->prototype[j] == 1)
            {
                result[added][currBond][0] = udata->graph->edges[j][0];
                result[added][currBond][1] = udata->graph->edges[j][1];
                ++currBond;
            }
        }
        result[added][currBond] = NULL;
        /*if enough independent cycles were found, break out of the loop*/
        if(++added == size) break;
    }
    free(mat);
    
    (*ptr) = result;
    return size;
}

int giveRCprototypes(urfdata *udata, int ****ptr)
{
    int ***result;
    int nofRel=0;
    int currFam=0, currEdge;
    int i,j;
    cfURF *CFs;
    if(udata->nofURFs < 1) return 0;
    
    CFs=udata->CFs;
    /*find number of relevant cycle families*/
    for(i=0; i<CFs->nofFams; ++i)
    {
        if(CFs->fams[i]->mark > 0)
        {
            ++nofRel;
        }
    }
    /*allocate space*/
    result = malloc(nofRel * sizeof(*result));
    for(i=0; i<CFs->nofFams; ++i)
    {
        if(CFs->fams[i]->mark > 0)
        {/*write new cycle*/
            result[currFam] = alloc2DIntArray(CFs->fams[i]->weight + 1,2);
            currEdge = 0;
            for(j=0; j<udata->graph->E; ++j)
            {/*copy prototype into new structure*/
                if(CFs->fams[i]->prototype[j] == 1)
                {
                    result[currFam][currEdge][0] = udata->graph->edges[j][0];
                    result[currFam][currEdge][1] = udata->graph->edges[j][1];
                    ++currEdge;
                }
            }
            result[currFam][currEdge] = NULL; /*end of cycle*/
            ++currFam;
        }
    }
    
    (*ptr) = result;
    return nofRel;
}

int giveRCcycles(urfdata *udata, int ****ptr)
{
    int ***result;
    int ***URFrel; /*relevant Cycles of a URF*/
    int i,j;
    int alloced=4, nextfree=0;
    int num;
    if(udata->nofURFs < 1) return 0;
    
    result = malloc(alloced * sizeof(*result));
    for(i=0; i<udata->nofURFs; ++i)
    {
        num = giveURFCycles(udata, &URFrel, i);
        for(j=0; j<num; ++j)/*append all cycles of URF i to result*/
        {
            if(alloced == nextfree)/*more space needed*/
            {
                alloced *= 2;
                result = realloc(result, alloced * sizeof(*result));
            }
            result[nextfree++] = URFrel[j];
        }
        free(URFrel);
    }
    
    (*ptr) = result;
    return nextfree;
}

void deleteBondArr(int **arr)
{
    delete2DArray((void **)arr);
}

int translateCycleArray(urfdata *udata, int ***array, int number, char ***ptr)
{
    int i,j,edgeIdx;
    char **result;
    result = alloc2DCharArray(number, udata->graph->E);
    for(i=0; i<number;  ++i)/*initialize to 0s*/
    {
        for(j=0; j<udata->graph->E; ++j)
        {
            result[i][j] = 0;
        }
    }
    
    for(i=0; i<number; ++i)
    {
        for(j=0; array[i][j]!=NULL; ++j)
        {
            edgeIdx = edgeId(udata->graph, array[i][j][0], array[i][j][1]);
            result[i][edgeIdx] = 1;
        }
    }
    (*ptr) = result;
    return number;
}

void deleteEdgeIdxCycles(char **cycles)
{
    delete2DArray((void **)cycles);
}
