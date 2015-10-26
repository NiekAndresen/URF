/* Niek Andresen 2015 - Bachelor Thesis */

#include "CycleFamsURF.h"

void URF_swap(cfam **arr, int idx1, int idx2)
{
    cfam *temp;
    temp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = temp;
}

int URF_partition(cfam **arr, int low, int high)
{
    int i,position = low;
    int pivotIdx = (low+high)/2;
    int pivotElem = arr[pivotIdx]->weight;
    URF_swap(arr, pivotIdx, high);
    for(i=low; i<high; ++i)
    {
        if(arr[i]->weight < pivotElem)
        {
            URF_swap(arr, i, position++);
        }
    }
    URF_swap(arr, position, high);
    return position;
}

void URF_quicksort(cfam **arr, int low, int high)
{
    int separationIdx;
    if(low < high)
    {
        separationIdx = URF_partition(arr, low, high);
        URF_quicksort(arr, low, separationIdx-1);
        URF_quicksort(arr, separationIdx+1, high);
    }
}

void URF_sortbyweight(cfURF *rc)
{
    URF_quicksort(rc->fams, 0, rc->nofFams-1);
}
