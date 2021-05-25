#ifndef _EDGE_H_
#define _EDGE_H_

#include "graphs.h"

edge_t *edge_heap_pop(edge_t **heap);
void edge_heap_push(edge_t *node, edge_t **heap);
void edge_heap_sort(edge_t **heap, size_t size);
void edge_heapify(edge_t **heap, size_t i, size_t size);

#endif /* _EDGE_H */
