#ifndef _dijkstra_H_
#define _dijkstra_H_

#include "graphs.h"

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
} dijkstra_node_t;

dijkstra_node_t *dijkstra_heap_pop(dijkstra_node_t **heap);
void dijkstra_heap_push(dijkstra_node_t *node, dijkstra_node_t **heap);
void dijkstra_heap_sort(dijkstra_node_t **heap, size_t size);
void dijkstra_heapify(dijkstra_node_t **heap, size_t i, size_t size);
dijkstra_node_t *dijkstra_node_init(const vertex_t *vertex,
	dijkstra_node_t *via, int weight);






#endif /* _dijkstra_H_ */
