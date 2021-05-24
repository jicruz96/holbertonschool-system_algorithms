#ifndef _DK_H_
#define _DK_H_

#include "graphs.h"


/**
 * union dijkstra_node_self_s - union for self attribute of dijkstra_node_t
 * @vertex: vertex struct (see graphs.h)
 * @edge: edge struct (see graphs.h)
 **/
typedef union dijkstra_node_self_s
{
	const vertex_t *vertex;
	const edge_t *edge;
} dk_node_self_t;

/**
 * struct dijkstra_node_s - struct with information about a graph node
 *                          to help us perform dijkstra's Algorithm
 * @vertex: current position in graph (vertex_t)
 * @via: previous position in graph (dijkstra_node_s)
 * @weight: weight of path from start point
 */
typedef struct dijkstra_node_s
{
	const vertex_t *vertex;
	struct dijkstra_node_s *via;
	int weight;
} dk_node_t;

dk_node_t *dk_heap_pop(dk_node_t **heap);
void dk_heap_push(dk_node_t *node, dk_node_t **heap);
void dk_heap_sort(dk_node_t **heap, size_t size);
void dk_heapify(dk_node_t **heap, size_t i, size_t size);
dk_node_t *dk_node_init(const vertex_t *vertex, dk_node_t *via, int weight);





#endif /* _DK_H_ */
