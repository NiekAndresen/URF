/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>
#include "URFrelation.h"
#include "URFInfo.h"
#include "graphURF.h"
#include "CycleFamsURF.h"
#include "io.h"

void swapRows(char **mat, int row1, int row2, int maxCol)
{
    int temp, i;
    for(i=0; i<=maxCol; ++i)
    {
        temp = mat[row1][i];
        mat[row1][i] = mat[row2][i];
        mat[row2][i] = temp;
    }
}

/** Eliminates the entry mat[row2][row] by adding the row with index 'row' to the the row with index 'row2'. Returns 0 if it created a row with only zeros. */
int eliminateRow(char **mat, int row, int row2, int maxCol)
{
    int i, result=1;
    for(i=row; i<=maxCol; ++i)
    {
        if( (mat[row][i] == 1 && mat[row2][i] != 1) || (mat[row][i] != 1 && mat[row2][i] == 1) )
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

/** does Gaussian elimination on the matrix with rows and columns ranging from index 0 to maxRow. Returns 1 if the rows are linearly dependent and 0 if not. */
int dependent(char **inMat, int maxRow, int maxCol)
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

if(maxRow == 9)
{
    printf("in dependency check:\n");
    printf("matrix looks like this:\n");
    printmat(mat, maxRow, maxCol);
}
    
    col = 0;
    for(row=0; row<=maxRow; ++row)
    {
        /*swap row with entry == 1 in current column (on diagonal) to current row*/
        for(row2=row; row2<=maxRow; ++row2)
        {
            if(mat[row2][col] == 1)
            {
                break;
            }
        }
        if(row2 == maxRow + 1)/*there is no row with an entry in this column*/
        {
            ++col;
            --row;
            continue;
        }
        swapRows(mat, row, row2, maxCol);
        
        /*eliminate following rows*/
        for(row2=row+1; row2<=maxRow; ++row2)
        {
            if(mat[row2][col] == 1)
            {
if(maxRow == 9)
{
    printf("eliminating %d with %d to maxCol=%d\n",row2,row,maxCol);
}
                indep = eliminateRow(mat, row, row2, maxCol);
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
if(maxRow == 9)
{
    printf("step:\n");
    printmat(mat, maxRow, maxCol);
}
    }
    free(mat[0]);
    free(mat);
    return (1 - indep);
}

/** returns the index in the array of RCFs (fams) that the RCF with the weight at the index "weight" and position j has */
int idxWeight(URFinfo *uInfo, int weight, int j)
{
    int i, sum = 0;
    for(i=0; i<weight; ++i)
    {
        sum += uInfo->nofProtos[i];
    }
    return sum + j;
}

/** Writes into the URFinfo if two RCFs are potentially URF-related because of linear dependencies (condition 3 URF-relation) */
/* 3 sets are maintained (see also Vismara):
    'B' is the current set of basis prototypes (corresponding to a RCF)
    'B<' is the set of cycles in B which are shorter than the cycles we are looking at at that moment. RCFs in 'B<' are marked with 1.
    'B=' is the set of cycles in B which have the same length as the cycles we are looking at at that moment. RCFs in 'B=' are marked with 2.
To see whether or not two RCFs are potentially URF-related we look at each RCF 'C' of each weight individually:
    If C is independent of 'B<' we look at it's dependency on all combinations of 'B<' with one of 'B='.
    If it is independent of all those combinations we look at if it is independent of the combination of 'B<' and all of 'B='.
    If this is the case, it becomes part of the basis B (part of 'B=')
    If we find that it is dependent on 'B<' and one of 'B=', the element of 'B=' and C are marked as potentially URF-related.*/
void checkDependencies(cfURF *RCFs, GraphURF *graph, URFinfo *uInfo)
{
    char **matrix;
    int currRow; /*index of the first row that does NOT contain a cycle that is part of "B=" (see Vismara)*/
    int i,j,k,testRow=0;
    int numAdded; /*how many cycles were added to 'B' within the currently considered weight*/
    char indepOfAll; /*flag storing if a cycle was independet of all combinations of 'B<' with one of 'B='*/
    if(RCFs->nofFams < 3) return;

    matrix = malloc(RCFs->nofFams * sizeof(*matrix));
    currRow = 0;

    for(i=0; i<uInfo->nofWeights; ++i)/*for each weight (index)*/
    {
        numAdded = 0;
        for(j=0; j<uInfo->nofProtos[i]; ++j)/*for each CF with this weight*/
        {
if(idxWeight(uInfo,i,j) == 8)
{
    printf("we are at no 8\n");
}
            matrix[currRow] = RCFs->fams[idxWeight(uInfo, i, j)]->prototype;/*add prototype to matrix*/
            if(dependent(matrix, currRow, graph->E-1) == 0)/*independent of "B<" (see e.g. Vismara)*/
            {/* check potential URF-relations to other cycles of the same length ("weight") */
if(idxWeight(uInfo,i,j) == 8)
{
    printf("no 8 was indep of B<\n");
}
if(idxWeight(uInfo,i,j) == 10)
{
    printf("no 10 was indep of B<\n");
}
                for(k=0; k<uInfo->nofProtos[i]; ++k)
                {
                    indepOfAll = 'y';
                    if(RCFs->fams[idxWeight(uInfo, i, k)]->mark == 2) /* if other cycle is marked as being in the set 'B=' */
                    {
                        matrix[currRow+1] = RCFs->fams[idxWeight(uInfo, i, k)]->prototype;

if(idxWeight(uInfo,i,j) == 10)
{
    printf("9 should be in B= and now the dependence of B<, 9 and 10 is checked.\n");
    printf("with the call dependent(matrix, %d, %d)\n",currRow+1, graph->E-1);
}
                        uInfo->URFs[i][j][k] = dependent(matrix, currRow+1, graph->E-1);
                        uInfo->URFs[i][k][j] = uInfo->URFs[i][j][k]; /*make matrix symmetric*/
if(idxWeight(uInfo,i,j) == 10)
{
    printf("the result was %d\n",uInfo->URFs[i][j][k]);
}
                        if(uInfo->URFs[i][j][k] == 1)
                        {
                            indepOfAll = 'n';
                        }
                    }
                RCFs->fams[idxWeight(uInfo,i,j)]->mark = 1; /*was independent of smaller cycles, so it is relevant*/
                }
            }
            if(indepOfAll == 'y')/*if the cycle was independent of all combinations of 'B<' with one of 'B='*/
            {
                /*attach all of 'B=' to the matrix*/
                testRow = currRow+1;
                for(k=0; k<uInfo->nofProtos[i]; ++k)
                {
                    if(RCFs->fams[idxWeight(uInfo, i, k)]->mark == 2)
                    {
                        matrix[testRow++] = RCFs->fams[idxWeight(uInfo, i, k)]->prototype;
                    }
                }
                /*check if the new cycle is indep of all of 'B<' and 'B=' combined*/
                if(dependent(matrix, testRow-1, graph->E-1) == 0)
                {
                    RCFs->fams[idxWeight(uInfo, i, j)]->mark = 2;
                    ++numAdded;
                }
            }
        }
        /*all cycles in 'B=' become part of 'B<'*/
        for(j=0; j<uInfo->nofProtos[i]; ++j)
        {
            if(RCFs->fams[idxWeight(uInfo, i, j)]->mark == 2)
            {
                RCFs->fams[idxWeight(uInfo, i, j)]->mark = 1;
            }
        }
        currRow += numAdded;
    }
    
    free(matrix);
}

/*finds the index of the edge [from, to] in the graph*/
int edgeIdx(int from, int to, GraphURF *gra)
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
        if((gra->edges[edge][0] == from) && (gra->edges[edge][1] == to))
        {
            break;
        }
    }
    return edge;
}

/** not unlike the function "List_Paths" from Vismara, this function finds all edges that are on shortest paths from r to x recursively and stores the result in the array edges.*/
void recFinder(int x, int r, int *edges, GraphURF *gra, sPathInfo *spi)
{
    int i, vertex;
    
    if(x == r) return;
    for(i=0; i<spi->dPaths[r]->degree[x]; ++i)/*each vertex adjacent to x in U_r*/
    {
        vertex = spi->dPaths[r]->adjList[x][i];
printf("trying to go from %d to %d in U_%d\n",x,vertex,r);
        edges[edgeIdx(x, vertex, gra)] = 1;
        recFinder(vertex, r, edges, gra, spi);
    }
}

/** finds all edges in the RCF and stores the result in the array edges.*/
void findEdges(int *edges, cfam *RCF, GraphURF *gra, sPathInfo *spi)
{
    recFinder(RCF->p, RCF->r, edges, gra, spi);
    recFinder(RCF->q, RCF->r, edges, gra, spi);
} 

/** Checks if the RCFs with indices idx1 and idx2 share at least one edge. returns 1 if yes and 0 otherwise. */
int shareEdges(cfURF *RCFs, int idx1, int idx2, GraphURF *graph, sPathInfo *spi)
{
    int *edges1;
    int *edges2; /*arrays of {0,1}^m that can store a set of edges with entries being 1 if the edge is contained and 0 otherwise*/
    int i;
    int result = 0;
    edges1 = malloc(graph->E * sizeof(*edges1));
    edges2 = malloc(graph->E * sizeof(*edges2));
    for(i=0; i<graph->E; ++i)
    {
        edges1[i]=0;
        edges2[i]=0;
    }
    findEdges(edges1, RCFs->fams[idx1], graph, spi);
    findEdges(edges2, RCFs->fams[idx2], graph, spi);
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

/** checks the previously 'marked as potentially URF-related' RCFs for edges that they have in common which proves that they are URF-related. */
void checkEdges(cfURF *RCFs, GraphURF *graph, URFinfo *uInfo, sPathInfo *spi)
{
    int i,j,k;
    for(i=0; i<uInfo->nofWeights; ++i) /*go through all matrices*/
    {
        for(j=0; j<uInfo->nofProtos[i]; ++j)
        {
            for(k=j+1; k<uInfo->nofProtos[i]; ++k) /*since matrix is symmetric: only look into top right half*/
            {
                if(uInfo->URFs[i][j][k] == 1) /*find entries which indicate potential URF-relations*/
                {
                    uInfo->URFs[i][j][k] = shareEdges(RCFs, idxWeight(uInfo, i, j), idxWeight(uInfo, i, k), graph, spi); /*1 if RCFs j and k share at least an edge*/
                    uInfo->URFs[i][k][j] = uInfo->URFs[i][j][k];
                }
            }
        }
    }
}

void findRelations(cfURF *RCFs, GraphURF *graph, URFinfo *uInfo, sPathInfo *spi)
{
    checkDependencies(RCFs, graph, uInfo);
    //checkEdges(RCFs, graph, uInfo, spi); //needed? Seems like it's unnecessary.
}

