/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "URFrelation.h"
#include "URFinfo.h"
#include "URFgraph.h"
#include "CycleFamsURF.h"
#include "URFhandler.h"
#include "utility.h"

void URF_swapRows(char **mat, int row1, int row2, int maxCol)
{
  int temp, i;
  for(i=0; i<=maxCol; ++i)
  {
    temp = mat[row1][i];
    mat[row1][i] = mat[row2][i];
    mat[row2][i] = temp;
  }
}

/** Eliminates the entry mat[row2][row] by adding the row with index 'row' to
the the row with index 'row2'. Returns 0 if it created a row with only zeros. */
int URF_eliminateRow(char **mat, int row, int row2, int maxCol)
{
  int i, result=1;
  for(i=row; i<=maxCol; ++i)
  {
    if( (mat[row][i] == 1 && mat[row2][i] != 1) ||
        (mat[row][i] != 1 && mat[row2][i] == 1) )
    {/*exactly one of the entries is 1*/
      mat[row2][i] = 1; 
    }
    else
    {
      mat[row2][i] = 0;
    }
  }
  for(i=0; i<=maxCol; ++i)
  {
    if(mat[row2][i] == 1)
    {
      break;
    }
  }
  if(i == maxCol+1)
  {
    result = 0;
  }
  return result;
}

/** does Gaussian elimination on the matrix with rows and columns ranging from
index 0 to maxRow. Returns 1 if the rows are linearly URF_dependent and 0 if
not. */
int URF_dependent(char **inMat, int maxRow, int maxCol)
{
  char **mat;
  int row, row2 , col, i, indep=1;
  
  if(maxRow < 2)
  {
    return 0;
  }

  /*make a copy of the matrix*/
  mat = malloc((maxRow+1) * sizeof(*mat));
  mat[0] = malloc((maxRow+1) * (maxCol+1) * sizeof(**mat));
  for(i=1; i<=maxRow; ++i)
  {
    mat[i] = mat[i-1] + (maxCol+1);
  }
  for(row=0; row<=maxRow; ++row)
  {
    for(col=0; col<=maxCol; ++col)
    {
      mat[row][col] = inMat[row][col];
    }
  }
  
  col = 0;
  for(row=0; row<=maxRow; ++row)
  {
    /*URF_swap row with entry == 1 in current column (on diagonal) to current
    row*/
    for(row2=row; row2<=maxRow; ++row2)
    {
      if(mat[row2][col] == 1)
      {
        break;
      }
    }
    if(row2 == maxRow + 1)/*there is no row with an entry in this column*/
    {/*skip this column*/
      ++col;
      --row;
      continue;
    }
    URF_swapRows(mat, row, row2, maxCol);
    
    /*eliminate following rows*/
    for(row2=row+1; row2<=maxRow; ++row2)
    {
      if(mat[row2][col] == 1)
      {
        indep = URF_eliminateRow(mat, row, row2, maxCol);
        if(indep == 0)
        {
          break;
        }
      }
    }
    if(indep == 0)
    {
      break;
    }
  }
  free(mat[0]);
  free(mat);
  return (1 - indep);
}

int URF_linDep(char **mat, int rows, int cols)
{
  return URF_dependent(mat, rows-1, cols-1);
}

/** returns the index in the array of RCFs (fams) that the RCF with the weight
at the index "weight" and position j has */
int URF_idxWeights(URFinfo *uInfo, int weight, int j)
{
  int i, sum = 0;
  for(i=0; i<weight; ++i)
  {
    sum += uInfo->nofProtos[i];
  }
  return sum + j;
}

/** Writes into the URFinfo if two RCFs are potentially URF-related because of
linear dependencies (condition 3 URF-relation). In this step all relevant cycle
families are marked as well.*/
/* 3 sets are maintained (see also Vismara):
  'B' is the current set of basis prototypes (corresponding to a RCF)
  'B<' is the set of cycles in B which are shorter than the cycles we are
  looking at at that moment. RCFs in 'B<' are marked with 1.
  'B=' is the set of cycles in B which have the same length as the cycles we are
  looking at at that moment. RCFs in 'B=' are marked with 2 or 3.
To see whether or not two RCFs are potentially URF-related we look at each RCF
'C' of each weight individually:
  If C is independent of 'B<' we look at it's dependency on all combinations
  of 'B<' with one of 'B='.
  If it is independent of all those combinations we look at if it is independent
  of the combination of 'B<' and all of 'B='.
  If this is the case, it becomes part of the basis B (part of 'B=')
  If we find that it is URF_dependent on 'B<' and one of 'B=', the element of
  'B=' and C are marked as potentially URF-related.*/
void URF_checkDependencies(cfURF *RCFs, URF_graph *graph, URFinfo *uInfo)
{
  char **matrix;
  int currRow; /*index of the first row that does NOT contain a cycle that is
               part of "B=" (see Vismara)*/
  int i,j,k,testRow=0,idx;
  char indepOfAll; /*flag storing if a cycle was independet of all combinations
                    of 'B<' with one of 'B='*/
  char *temp;
  
  if(RCFs->nofFams < 3)
  {/*if only 0, 1 or 2 families exist, they are all relevant and independent*/
    for(i=0; i<uInfo->nofWeights; ++i)
    {
      for(j=0; j<uInfo->nofProtos[i]; ++j)
      {
        uInfo->URFrel[i][j][j] = 1; /*URF-related to itself*/
      }
    }
    for(i=0; i<RCFs->nofFams; ++i)
    {
      RCFs->fams[i]->mark = 1;
    }
    return;
  }
  
  matrix = malloc(RCFs->nofFams * sizeof(*matrix));
  currRow = 0;

  for(i=0; i<uInfo->nofWeights; ++i)/*for each weight (index)*/
  {
    for(j=0; j<uInfo->nofProtos[i]; ++j)/*for each CF with this weight*/
    {
      /*add prototype to matrix*/
      matrix[currRow] = RCFs->fams[URF_idxWeights(uInfo, i, j)]->prototype;
      /*if independent of "B<" (see e.g. Vismara)*/
      if(URF_dependent(matrix, currRow, graph->E-1) == 0)
      {/*check potential URF-relations to other cycles of the same weight */
        uInfo->URFrel[i][j][j] = 1; /*URF-related to itself*/
        indepOfAll = 'y';
        for(k=0; k<uInfo->nofProtos[i]; ++k)
        {
          if(RCFs->fams[URF_idxWeights(uInfo, i, k)]->mark > 1) /* if other
          cycle is marked as being in the set 'B='*/
          {
            matrix[currRow+1] = RCFs->fams[URF_idxWeights(uInfo, i, k)]->prototype;
            uInfo->URFrel[i][j][k] = URF_dependent(matrix, currRow+1, graph->E-1);
            /*make matrix symmetric*/
            uInfo->URFrel[i][k][j] = uInfo->URFrel[i][j][k];
            if(uInfo->URFrel[i][j][k] == 1)
            {
              indepOfAll = 'n';
            }
          }
        }
        RCFs->fams[URF_idxWeights(uInfo,i,j)]->mark = 3; /*was independent
        of smaller cycles, so it is relevant, but not necessarily independent of
        all of 'B<' and 'B=' combined (mark 2) (which is checked below)*/
        /*if the cycle was independent of all combinations of 'B<' with one of
        'B='*/
        if(indepOfAll == 'y')
        {
          /*attach all of 'B=' to the matrix*/
          temp = matrix[currRow];
          testRow = currRow;
          for(k=0; k<uInfo->nofProtos[i]; ++k)
          {
            if(k!=j && RCFs->fams[URF_idxWeights(uInfo, i, k)]->mark == 2)
            {
              matrix[testRow++] = RCFs->fams[URF_idxWeights(uInfo, i, k)]->prototype;
            }
          }
          matrix[testRow++] = temp;
          /*check if the new cycle is indep of all of 'B<' and 'B=' combined*/
          if(URF_dependent(matrix, testRow-1, graph->E-1) == 0)
          {
            RCFs->fams[URF_idxWeights(uInfo, i, j)]->mark = 2;
          }
        }
      }
    }
    /*all cycles in 'B=' become part of 'B<'*/
    for(j=0; j<uInfo->nofProtos[i]; ++j)
    {
      idx = URF_idxWeights(uInfo, i, j);
      if(RCFs->fams[idx]->mark == 2)
      {
        matrix[currRow++] = RCFs->fams[idx]->prototype;
        RCFs->fams[idx]->mark = 1;
      }
    }
  }
  
  /*all marks > 0 are set to 1*/
  for(i=0; i<RCFs->nofFams; ++i)
  {
    if(RCFs->fams[i]->mark > 1)
    {
      RCFs->fams[i]->mark = 1;
    }
  }
  
  free(matrix);
}

/** not unlike the function "List_Paths" from Vismara, this function finds all
edges that are on shortest paths from r to x recursively and stores the result
in the array edges.*/
void URF_recFinder(int x, int r, int *edges, URF_graph *gra, sPathInfo *spi)
{
  int i, vertex;
  
  if(x == r) return;
  /*for each vertex URF_adjacent to x in U_r*/
  for(i=0; i<spi->dPaths[r]->degree[x]; ++i)
  {
    vertex = spi->dPaths[r]->adjList[x][i];
    edges[URF_edgeId(gra, x, vertex)] = 1;
    URF_recFinder(vertex, r, edges, gra, spi);
  }
}

/** finds all edges in the RCF and stores the result in the array edges.*/
void URF_findEdges(int *edges, cfam *RCF, URF_graph *gra, sPathInfo *spi)
{
  URF_recFinder(RCF->p, RCF->r, edges, gra, spi);
  URF_recFinder(RCF->q, RCF->r, edges, gra, spi);
  if(RCF->x < INT_MAX) /*even family*/
  {
    edges[URF_edgeId(gra, RCF->x, RCF->p)] = 1;
    edges[URF_edgeId(gra, RCF->x, RCF->q)] = 1;
  }
  else
  {
    edges[URF_edgeId(gra, RCF->p, RCF->q)] = 1;
  }
} 

/** Checks if the RCFs with indices idx1 and idx2 share at least one edge.
returns 1 if yes and 0 otherwise. */
char URF_shareEdges(cfURF *RCFs, int idx1, int idx2, URF_graph *graph,
                    sPathInfo *spi)
{
  int *edges1;
  int *edges2; /*arrays of {0,1}^m that can store a set of edges with entries
  being 1 if the edge is contained and 0 otherwise*/
  int i;
  char result = 0;
  edges1 = malloc(graph->E * sizeof(*edges1));
  edges2 = malloc(graph->E * sizeof(*edges2));
  for(i=0; i<graph->E; ++i)
  {
    edges1[i]=0;
    edges2[i]=0;
  }
  URF_findEdges(edges1, RCFs->fams[idx1], graph, spi);
  URF_findEdges(edges2, RCFs->fams[idx2], graph, spi);

  for(i=0; i<graph->E; ++i)
  {
    if(edges1[i] == 1 && edges2[i] == 1)
    {
      result = 1;
      break;
    }
  }
  free(edges1);
  free(edges2);
  return result;
}

/** checks the previously 'marked as potentially URF-related' RCFs for edges 
that they have in common which proves that they are URF-related. */
void URF_checkEdges(cfURF *RCFs, URF_graph *graph, URFinfo *uInfo, sPathInfo *spi)
{
  int i,j,k,l;
  for(i=0; i<uInfo->nofWeights; ++i) /*go through all matrices*/
  {
    for(j=0; j<uInfo->nofProtos[i]; ++j)
    {
      for(k=j+1; k<uInfo->nofProtos[i]; ++k) /*since matrix is symmetric: only
                                              look into top right half*/
      {
        if(uInfo->URFrel[i][j][k] == 1) /*find entries which indicate potential
                                        URF-relations*/
        {
          uInfo->URFrel[i][j][k] = 0;
          uInfo->URFrel[i][k][j] = 0;
          for(l=0; l<graph->E;  ++l)
          {
            if(RCFs->fams[URF_idxWeights(uInfo,i,j)]->prototype[l] == 1 &&
               RCFs->fams[URF_idxWeights(uInfo,i,k)]->prototype[l] == 1)
            {/* they share an edge */
              uInfo->URFrel[i][j][k] = 1;
              uInfo->URFrel[i][k][j] = 1;
            }
          }
        }
      }
    }
  }
}

/** Takes the matrices in "URFrel" which contain the URF-pair-relation and finds
the transitive closure - the URF-relation. */
void URF_findTransitiveClosure(URFinfo *uInfo)
{
  int i,j,k,l;
  for(i=0; i<uInfo->nofWeights; ++i)
  {
    for(j=0; j<uInfo->nofProtos[i]; ++j)
    {/*for each pair of 1s in a line, the two families have to be marked as
     URF-related*/
      for(k=0; k<uInfo->nofProtos[i]; ++k)
      {
        for(l=k+1; l<uInfo->nofProtos[i]; ++l)
        {
          if(uInfo->URFrel[i][j][k] == 1 && uInfo->URFrel[i][j][l] == 1)
          {
            uInfo->URFrel[i][k][l] = 1;
            uInfo->URFrel[i][l][k] = 1;
          }
        }
      } 
    }
  }
}

void URF_findRelations(cfURF *RCFs, URF_graph *graph, URFinfo *uInfo,
                       sPathInfo *spi)
{
  URF_checkDependencies(RCFs, graph, uInfo);
  URF_checkEdges(RCFs, graph, uInfo, spi);
  URF_findTransitiveClosure(uInfo);
}

