#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "heap.h"

typedef struct{
    int id; //i.e. the vertex index
    int key; //i.e. the dist
}heap_node;

typedef struct{
    int maxIdx;
    heap_node **array; //heap array containing pointers to nodes
    int *arr_idx; //array that stores for each id where its node is in the heap array
}heap;

void swap_nodes(heap *hea, int idx1, int idx2)
{
    heap_node *temp;
    
    temp=hea->array[idx1];
    hea->array[idx1]=hea->array[idx2];
    hea->array[idx2]=temp;
    hea->arr_idx[hea->array[idx1]->id] = idx1;
    hea->arr_idx[hea->array[idx2]->id] = idx2;
}

void heapify(heap *hea, int idx)
{
    int idx_smallest = idx;
    
    if(idx*2+1 > hea->maxIdx) //leaf
    {
        return;
    }
    
    if(hea->array[idx*2+1]->key < hea->array[idx_smallest]->key)
    {
        idx_smallest = idx*2+1; //left child
    }
    if(idx*2+2 <= hea->maxIdx && hea->array[idx*2+2]->key < hea->array[idx_smallest]->key)
    {
        idx_smallest = idx*2+2; //right child
    }
    if(idx_smallest != idx)
    {
        swap_nodes(hea, idx, idx_smallest);
        heapify(hea, idx_smallest);
    }
}

void heapify_up(heap *hea, int idx)
{
    while(idx != 0)
    {
        heapify(hea, idx);
        idx = (idx-1) / 2; //parent
    }
}

int extract_min(heap *hea)
{
    assert(hea->maxIdx >= 0);
    int min = hea->array[0]->id;
    swap_nodes(hea, 0, hea->maxIdx);
    free(hea->array[hea->maxIdx]);
    --hea->maxIdx;
    heapify(hea, 0);
    
    return min;    
}

void decrease_key(heap *hea, int id, int new_key)
{
    hea->array[hea->arr_idx[id]]->key = new_key;
    heapify_up(hea, hea->arr_idx[id]);
}

heap *build_heap(int numOfElems, int *keys)
{
    int i;
    heap *hea = malloc(sizeof(*hea));
    heap_node **array = malloc(numOfElems * sizeof(*array));
    for(i=0; i<numOfElems; ++i)
    {
        array[i] = malloc(sizeof(*array[i]));
        array[i]->id = i;
        array[i]->key = keys[i];
    }
    hea->array = array;

    hea->maxIdx = numOfElems-1;
    hea->arr_idx = malloc(numOfElems * sizeof(*hea->arr_idx));
    for(i=0; i<numOfElems; ++i)
    {
        hea->arr_idx[i] = i;
    }
    
    for(i=numOfElems/2; i>=0; --i)
    {
        heapify(hea, i);
    }
    return hea;
}

void delete_heap(heap *hea)
{
    int i;
    for(i=0; i<=hea->maxIdx; ++i)
    {
        free(hea->array[i]);
    }
    free(hea->array);
    free(hea->arr_idx);
    free(hea);
}
