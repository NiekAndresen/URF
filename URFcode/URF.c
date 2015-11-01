/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../URF.h"
#include "apsp.h"
#include "CycleFamsURF.h"
#include "URFhandler.h"
#include "utility.h"
#include "URFgraph.h"
#include "URFrelation.h"
#include "URFinfo.h"

URF_graph *URF_initNewGraph(unsigned int V)
{
  return URF_initNewGraph_g(V);
}

void URF_addUEdge(URF_graph *gra, URF_atom from, URF_atom to)
{
  if(from >= gra->V || to >= gra->V)
  {
    URF_warn('a');
    fprintf(stderr,
    "edge (%d,%d) can not be added to graph with %d atoms - was ignored.\n",
    from, to, gra->V);
    return;
  }
  URF_addUEdge_g(gra, from, to);
}

void URF_findShortestPaths(URF_data *udata, URF_graph *gra)
{
  udata->spi = URF_AllPairsShortestPaths(gra);
}

URF_data *URF_calculate(URF_graph *gra)
{
  char correctGraph;
  URF_data *udata;
  
  correctGraph = URF_checkGraphCorrect(gra); /*from URF_graph.h*/
  if(correctGraph == 0) URF_warn('c'); /* from utility.h */
  udata = malloc(sizeof(*udata));
  URF_findShortestPaths(udata, gra); /*from apsp.h*/
  udata->CFs = URF_findCycleFams(gra, udata->spi); /*from CycleFamsURF.h*/
  udata->graph = gra;
  if(udata->CFs->nofFams > 0)
  {
    /*from URFinfo.h*/
    udata->urfInfo = URF_checkURFRelation(udata->CFs, udata->graph, udata->spi);
    udata->nofURFs = udata->urfInfo->nofURFs;
  }
  else
  {
    udata->nofURFs = 0;
  }
  return udata;
}

void URF_deleteData(URF_data *udata)
{
  URF_deleteAPSP(udata->spi, udata->graph->V);
  URF_deleteCycleFams(udata->CFs);
  if(udata->nofURFs > 0)
  {
    URF_deleteURFInfo(udata->urfInfo);
  }
  URF_deleteGraph(udata->graph);
  free(udata);
}

unsigned int URF_getNumber(URF_data *udata)
{
  return (unsigned int)udata->nofURFs;  
}

unsigned int URF_getWeight(URF_data *udata, int index)
{
  if(udata->nofURFs < 1 || index >= udata->nofURFs) return 0;
  return udata->urfInfo->URFs[index][0]->weight;
}

/** gives an array of indices of atoms that are contained in the URF with the
given index. Array is terminated by INT_MAX */
URF_atom *getAtoms(URF_data *uData, int index)
{
  int i,nofFams,nextfree=0,alloced;
  char *atoms;
  URF_atom *result;
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
    URF_giveVertices(URF[i]->r, URF[i]->q, atoms, uData->graph, uData->spi);
    URF_giveVertices(URF[i]->r, URF[i]->p, atoms, uData->graph, uData->spi);
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

/** gives an array of indices of bonds that are contained in the URF with the
given index. Array is terminated by INT_MAX */
unsigned int *getBonds(URF_data *uData, int index)
{
  int i,nofFams,nextfree=0,alloced;
  char *bonds;
  unsigned int *result;
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
    URF_giveEdges(URF[i]->r, URF[i]->q, bonds, uData->graph, uData->spi);
    URF_giveEdges(URF[i]->r, URF[i]->p, bonds, uData->graph, uData->spi);
    if(URF[i]->x < INT_MAX) /*even cycle*/
    {
      bonds[URF_edgeId(uData->graph,URF[i]->q,URF[i]->x)] = 1;
      bonds[URF_edgeId(uData->graph,URF[i]->p,URF[i]->x)] = 1;
    }
    else/*odd cycle*/
    {
      bonds[URF_edgeId(uData->graph,URF[i]->q,URF[i]->p)] = 1;
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

/** calls getAtoms() or getBonds() depending on mode 'a' or 'b' */
URF_atom *URF_giveURF(URF_data *uData, int URFindex, char mode)
{
  URF_atom *result;
  if(mode == 'a')
  {
    result = getAtoms(uData, URFindex);
  }
  else if(mode == 'b')
  {
    result = getBonds(uData, URFindex);
  }
  else
  {
    fprintf(stderr,
        "ERROR: tried to call 'URF_giveURF()' with invalid mode '%c'\n",mode);
    fprintf(stderr, "Only 'a' or 'b' are allowed. Read interface for help.\n");
    exit(EXIT_FAILURE);
  }

  return result;
}

unsigned int URF_getAtoms(URF_data *udata, unsigned int index, URF_atom **ptr)
{
  unsigned int i;
  if(udata->nofURFs < 1 || index >=udata->nofURFs)
  {
    /*to still be able to call 'free()' on the result*/
    (*ptr) = malloc(sizeof(**ptr));
    return 0;
  }
  (*ptr) = getAtoms(udata, index);
  for(i=0; (*ptr)[i]<INT_MAX; ++i); /*counts the number of atoms*/
  return i;
}

unsigned int URF_getBonds(URF_data *uData, unsigned int URFindex, URF_bond **ptr)
{
  unsigned int nextfree, alloced;
  URF_bond *result;
  unsigned int *bondIndices;
  unsigned int i;
  if(uData->nofURFs < 1 || URFindex >=uData->nofURFs)
  {
    /*to still be able to call 'deleteBondArray()' on the result*/
    (*ptr) = malloc(sizeof(**ptr));
    return 0;
  }

  nextfree = 0;
  alloced = 3;
  result = malloc(alloced * sizeof(*result));
  bondIndices = getBonds(uData, URFindex);
  for(i=0; bondIndices[i]<INT_MAX; ++i)
  {
    if(nextfree == alloced)/*more space needed in result*/
    {
      alloced *= 2; /* double the space */
      result = realloc(result, alloced * sizeof(*result));
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
  - 'a': A cycle is represented by an array of {0,1}^n which contains a 1 at
    position i if the vertex i is part of the cycle or 0 otherwise
    (n: number of vertices).
  - 'b': A cycle is represented by an array of {0,1}^m which contains a 1 at
    position i if the edge i is part of the cycle or 0 otherwise
    (m: number of edges).
The array of cycles is ended with a terminating NULL pointer.
This structure has to be deallocated using 'URF_deleteCyclesChar(<return value>)'.*/
char **URF_getCyclesChar(URF_data *udata, int index, char mode)
{
  int i;
  int currIdx=0; /*index of next free space in result array*/
  int alloced=2; /*space for how many cycles/paths has been allocated; is always
  a power of two. =2 in the beginning, grows with number of cycles*/
  int nofFams;
  cfam **URF;
  char **paths1, **paths2; /*much like the result array, but storing paths
  instead of cycles. paths1 for paths from r to p and paths2 for paths from r to
  q.*/
  char **result;
  
  nofFams = udata->urfInfo->nofCFsPerURF[index];
  URF = udata->urfInfo->URFs[index];
  
  result = malloc(alloced * sizeof(*result));
  for(i=0; i<nofFams; ++i)
  {
    URF_getPaths(URF[i]->r, URF[i]->p, &paths1, alloced, mode, udata->graph,
                udata->spi);
    URF_getPaths(URF[i]->r, URF[i]->q, &paths2, alloced, mode, udata->graph,
                udata->spi);
    currIdx = URF_combinePaths(&paths1, &paths2, URF[i]->p, URF[i]->q,
                    URF[i]->x, &result, currIdx, alloced, mode, udata->graph);
    /*calculate how many spaces are alloced with the help of the returned
    currIdx. Can be done since 'alloced' is always a power of two*/
    while(currIdx > alloced) alloced *= 2;
  }
  if(currIdx == alloced)
  {
    result = realloc(result, (alloced+1)*sizeof(*result));
  }
  result[currIdx] = NULL;
  return result;
}

/**to delete result of URF_getCyclesChar()*/
void URF_deleteCyclesChar(char **cycles)
{
  int i;
  for(i=0; cycles[i]!=NULL; ++i)
  {
    free(cycles[i]);
  }
  free(cycles);
}

unsigned int URF_getCycles(URF_data *udata, URF_cycle **ptr, unsigned int index)
{
  char **URF_cycles;
  URF_cycle *result;
  int i,j;
  unsigned int alloced, nextfree, nextBond;
  unsigned int edgeCount;
  if(udata->nofURFs < 1 || index >=udata->nofURFs) return 0;

  alloced = 4;
  nextfree = 0;
  result = malloc(alloced * sizeof(*result));
  URF_cycles = URF_getCyclesChar(udata, index, 'b');
  for(i=0; URF_cycles[i]!=NULL; ++i)
  {
    edgeCount = 0; /*count number of edges in this cycle*/
    for(j=0; j<udata->graph->E; ++j)
    {
      if(URF_cycles[i][j] == 1)
      {
        ++edgeCount;
      }
    }
    if(nextfree == alloced) /*allocate more space*/
    {
      alloced *= 2;
      result = realloc(result, alloced * sizeof(*result));
    }
    result[nextfree].bonds = malloc(edgeCount * sizeof(*result[nextfree].bonds));
    result[nextfree].weight = edgeCount;
    nextBond = 0;
    for(j=0; j<udata->graph->E; ++j)
    {
      if(URF_cycles[i][j] == 1) /*copy edge into new structure*/
      {
        result[nextfree].bonds[nextBond][0] = udata->graph->edges[j][0];
        result[nextfree].bonds[nextBond][1] = udata->graph->edges[j][1];
        ++nextBond;
      }
    }
    ++nextfree;
  }

  URF_deleteCyclesChar(URF_cycles);
  *ptr = result;
  return i;
}

void URF_deleteCycles(URF_cycle *cycles, unsigned int number)
{
  int i;
  if(number < 1) return;
  for(i=0; i<number; ++i)
  {
    free(cycles[i].bonds);
  }
  free(cycles);
}

/** Gives all URFs containing the object, which can be an atom or a bond.
mode:
  - 'a': atom
  - 'b': bond
returns an array of integers containing all indices of URFs containing the
object. The array ends with a terminating INT_MAX on its last position and has
to be deallocated with 'free()' */
unsigned int *URF_listURFs(URF_data *udata, int object, char mode)
{
  unsigned int *result;
  int *URFs; /*array of {0,1}^#URFs containing 1 on position i if the i-th URF
  contains the object*/
  int nextfree=0, alloced=2;
  unsigned int *objects;
  int i,j;
  char contained;
  
  if(!(mode == 'a' || mode == 'b'))
  {
    fprintf(stderr,
        "ERROR: tried to call 'URF_listURFs()' with invalid mode '%c'\n",mode);
    fprintf(stderr, "Only 'a' or 'b' are allowed. Read interface for help.\n");
    exit(EXIT_FAILURE);
  }
  URFs = malloc(udata->nofURFs * sizeof(*URFs));
  for(i=0; i<udata->nofURFs; ++i)
  {
    objects = URF_giveURF(udata, i, mode);
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

unsigned int URF_listURFsWithAtom(URF_data *udata, unsigned int **ptr,
                                      URF_atom object)
{
  unsigned int i;
  if(udata->nofURFs < 1)
  {
    /*to still be able to call free on result*/
    (*ptr) = malloc(sizeof(**ptr));
    return 0;
  }
  *ptr = URF_listURFs(udata, object, 'a');
  for(i=0; (*ptr)[i]<INT_MAX; ++i);
  return i;
}

unsigned int URF_listURFsWithBond(URF_data *udata, unsigned int **ptr,
                                      URF_atom atom1, URF_atom atom2)
{
  unsigned int i;
  if(udata->nofURFs < 1)
  {
    /*to still be able to call free on result*/
    (*ptr) = malloc(sizeof(**ptr));
    return 0;
  }
  *ptr = URF_listURFs(udata, URF_edgeId(udata->graph, atom1, atom2), 'b');
  for(i=0; (*ptr)[i]<INT_MAX; ++i);
  return i;
}

unsigned int URF_numContainingAtom(URF_data *udata, URF_atom atom)
{
  unsigned int number;
  unsigned int *arr;
  number = URF_listURFsWithAtom(udata, &arr, atom);
  free(arr);
  return number;
}

unsigned int URF_numContainingBond(URF_data *udata, URF_atom atom1, URF_atom atom2)
{
  unsigned int number;
  unsigned int *arr;
  number = URF_listURFsWithBond(udata, &arr, atom1, atom2);
  free(arr);
  return number;
}

unsigned int URF_getBasis(URF_data *udata, URF_cycle **ptr)
{
  int i,j;
  URF_cycle *result;
  char **mat; /*matrix in which the dependency checks take place*/
  unsigned int size, added;
  int currBond;
  if(udata->nofURFs < 1) return 0;

  size=udata->graph->E-udata->graph->V+1;
  mat = malloc(size * sizeof(*mat));
  result = malloc(size * sizeof(*result));
  
  added = 0;
  for(i=0; i<udata->nofURFs; ++i)
  {
    mat[added] = udata->urfInfo->URFs[i][0]->prototype;
    if(URF_linDep(mat, added+1, udata->graph->E) == 1) /*cycles are URF_dependent*/
    {
      /*cycle added last is not part of the basis -> will be ignored/overwritten
      in next step*/
      continue;
    }
    /*add cycle to result basis*/
    currBond = 0;
    result[added].bonds = malloc(udata->urfInfo->URFs[i][0]->weight *
                                 sizeof(*(result[added].bonds)));
    result[added].weight = udata->urfInfo->URFs[i][0]->weight;
    for(j=0; j<udata->graph->E; ++j)
    {
      if(udata->urfInfo->URFs[i][0]->prototype[j] == 1)
      {
        result[added].bonds[currBond][0] = udata->graph->edges[j][0];
        result[added].bonds[currBond][1] = udata->graph->edges[j][1];
        ++currBond;
      }
    }
    /*if enough inURF_dependent cycles were found, break out of the loop*/
    if(++added == size) break;
  }
  free(mat);
  
  (*ptr) = result;
  return size;
}

unsigned int URF_getRCPrototypes(URF_data *udata, URF_cycle **ptr)
{
  URF_cycle *result;
  unsigned int nofRel=0;
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
      result[currFam].bonds = malloc(CFs->fams[i]->weight *
                                     sizeof(*result[currFam].bonds));
      result[currFam].weight = CFs->fams[i]->weight;
      currEdge = 0;
      for(j=0; j<udata->graph->E; ++j)
      {/*copy prototype into new structure*/
        if(CFs->fams[i]->prototype[j] == 1)
        {
          result[currFam].bonds[currEdge][0] = udata->graph->edges[j][0];
          result[currFam].bonds[currEdge][1] = udata->graph->edges[j][1];
          ++currEdge;
        }
      }
      ++currFam;
    }
  }
  
  (*ptr) = result;
  return nofRel;
}

unsigned int URF_getRCycles(URF_data *udata, URF_cycle **ptr)
{
  URF_cycle *result;
  URF_cycle *URFrel; /*relevant Cycles of a URF*/
  int i,j;
  unsigned int alloced=4, nextfree=0;
  int num;
  if(udata->nofURFs < 1) return 0;
  
  result = malloc(alloced * sizeof(*result));
  for(i=0; i<udata->nofURFs; ++i)
  {
    num = URF_getCycles(udata, &URFrel, i);
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

unsigned int URF_translateCycArray(URF_data *udata, URF_cycle *array,
                                    unsigned int number, char ***ptr)
{
  int i,j,URF_edgeIdx;
  char **result;
  
  if(number < 1) return 0;
  result = URF_alloc2DCharArray(number, udata->graph->E);
  for(i=0; i<number;  ++i)/*initialize to 0s*/
  {
    for(j=0; j<udata->graph->E; ++j)
    {
      result[i][j] = 0;
    }
  }
  
  for(i=0; i<number; ++i)
  {
    for(j=0; j<array[i].weight; ++j)
    {
      URF_edgeIdx = URF_edgeId(udata->graph, array[i].bonds[j][0],
                                array[i].bonds[j][1]);
      result[i][URF_edgeIdx] = 1;
    }
  }
  (*ptr) = result;
  return number;
}

void URF_deleteEdgeIdxArray(char **cycles, unsigned int num)
{
  if(num < 1) return;
  URF_delete2DArray((void **)cycles);
}

unsigned int URF_getEdgeArray(URF_data *udata, URF_bond **ptr)
{
  URF_bond *result;
  unsigned int idx;
  
  result = malloc(udata->graph->E * sizeof(*result));
  for(idx=0; idx<udata->graph->E; ++idx)
  {
    result[idx][0] = udata->graph->edges[idx][0];
    result[idx][1] = udata->graph->edges[idx][1];
  }
  (*ptr) = result;
  return udata->graph->E;
}
