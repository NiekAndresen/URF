/* Niek Andresen 2015 - Bachelor Thesis */

#include "relevantCyclesURF.h"

void swap(rcf **arr, int idx1, int idx2)
{
    rcf *temp;
    temp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = temp;
}

int partition(rcf **arr, int low, int high)
{
    int i,position = low;
    int pivotIdx = (low+high)/2;
    int pivotElem = arr[pivotIdx]->weight;
    swap(arr, pivotIdx, high);
    for(i=low; i<high; ++i)
    {
        if(arr[i]->weight < pivotElem)
        {
            swap(arr, i, position++);
        }
    }
    swap(arr, position, high);
    return position;
}

void quicksort(rcf **arr, int low, int high)
{
    int separationIdx;
    if(low < high)
    {
        separationIdx = partition(arr, low, high);
        quicksort(arr, low, separationIdx-1);
        quicksort(arr, separationIdx+1, high);
    }
}

void sortbyweight(rcURF *rc)
{
    quicksort(rc->fams, 0, rc->nofFams-1);
}
