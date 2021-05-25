#ifndef _a_star_H_
#define _a_star_H_

#include "graphs.h"

/**
 * struct a_star_node_s - struct with information about a graph node
 *                          to help us perform a_star_'s Algorithm
 * @vertex: current position in graph (vertex_t)
 * @via: previous position in graph (a_star_node_s)
 * @weight: weight of path from start point
 * @distance: distance to target in graph
 */
typedef struct a_star_node_s
{
	const vertex_t *vertex;
	struct a_star_node_s *via;
	int weight;
	double distance;
} a_star_node_t;

a_star_node_t *a_star_heap_pop(a_star_node_t **heap);
void a_star_heap_push(a_star_node_t *node, a_star_node_t **heap);
void a_star_heap_sort(a_star_node_t **heap, size_t size);
void a_star_heapify(a_star_node_t **heap, size_t i, size_t size);
a_star_node_t *a_star_node_init(const vertex_t *vertex, a_star_node_t *via,
	int weight, const vertex_t *target);
double distance_to(const vertex_t *curr, const vertex_t *target);




#endif /* _a_star_H_ */
