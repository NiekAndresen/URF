/* Niek Andresen 2015 - Bachelor Thesis */

#include <stdlib.h>
#include <stdio.h>
#include "gaussElimURF.h"
#include "URFInfo.h"
#include "graphURF.h"
#include "relevantCyclesURF.h"

void swapRows(char **mat, int row1, int row2, int dim)
{
    int temp, i;
    for(i=0; i<dim; ++i)
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
    for(i=row; i<maxCol; ++i)
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
int gaussElim(char **inMat, int maxRow, int maxCol)
{
    char **mat;
    int row, row2 , col, i, indep=1;
    
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
    
    for(row=0; row<=maxRow; ++row)
    {
        /*swap row with entry == 1 in current column (on diagonal) to current row*/
        for(row2=row; row2<=maxRow; ++row2)
        {
            if(mat[row2][row] == 1)
            {
                break;
            }
        }
        if(row2 == maxRow + 1)
        {
            continue;
        }
        swapRows(mat, row, row2, maxCol);
        
        /*eliminate following rows*/
        for(row2=row+1; row2<=maxRow; ++row2)
        {
            if(mat[row2][row] == 1)
            {
                indep = eliminateRow(mat, row, row2, maxCol+1);
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

void findRelations(rcURF *RCFs, GraphURF *graph)//TODO in progress
{
    char **matrix;
    int currRow; /*index of the first row that does NOT contain a cycle that is part of "B=" (see Vismara)*/
    matrix = malloc(RCFs->nofFams * sizeof(*matrix));
    matrix[0] = RCFs->fams[0]->prototype;
    matrix[1] = RCFs->fams[1]->prototype;
    currRow = 2;
    matrix[2] = RCFs->fams[2]->prototype;
printf("starting Gauss\n");
    printf("dependent:%d\n",gaussElim(matrix, currRow, graph->E-1));
    free(matrix);
}

