#ifndef HEAP_H
#define HEAP_H

#include "heap.c"

/** A min-heap that stores elements with integer IDs and integer keys */

/** standard heapify */
void heapify(heap *hea, int idx);

/** function that is used by heapify to swap nodes */
void swap_nodes(heap *hea, int idx1, int idx2);

/** standard heapify to root (used by decrease_key) */
void heapify_up(heap *hea, int idx);

/** used to extract the node with the minimum key. Deletes this node from the heap and returns the ID */
int extract_min(heap *hea);

/** standard decrease key: find node, change key, heapify up */
void decrease_key(heap *hea, int id, int new_key);

/** builds and returns a heap of numOfElems Elements which get the ID from 0 to numOfElems-1. Their keys are given by the array "keys". "keys[0]" contains the key of the element with the ID 0. */
heap *build_heap(int numOfElems, int *keys);

/** deletes a heap */
void delete_heap(heap *hea);

#endif
