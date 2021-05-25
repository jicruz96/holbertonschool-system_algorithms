#include "pathfinding.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "heap.c"
#include "edge_compare.c"

static queue_t *make_result(a_star_node_t *node);
static int eval_neighbors(a_star_node_t *node, heap_t *edge_heap,
		a_star_node_t **seen, heap_t *heap, const vertex_t *target);
static int a_star_compare(void *vertex1, void *vertex2);
static a_star_node_t *a_star_node_init(const vertex_t *vertex,
		a_star_node_t *via, int weight, const vertex_t *target);

/**
 * a_star_node_init - initializes a a_star_node_t data structure
 * @vertex: current position in graph (vertex_t)
 * @via: previous postion in graph (a_star_node_t)
 * @weight: weight of path from start to curr via prev
 * @target: pointer to target node (used to calculate distance)
 * Return: pointer to new node
 */
static a_star_node_t *a_star_node_init(const vertex_t *vertex,
		a_star_node_t *via, int weight, const vertex_t *target)
{
	a_star_node_t *node = malloc(sizeof(a_star_node_t));

	if (node)
	{
		node->vertex = vertex;
		node->via = via;
		node->weight = weight;
#define distance_to(a, b) (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)))
		node->distance = distance_to(vertex, target);
	}

	return (node);
}



/**
 * a_star_graph - searches for the shortest path from a starting point to a
 *                  target point in a graph.
 * @graph: pointer to the graph to go through
 * @start: pointer to the starting vertex
 * @target: pointer to the target vertex
 * Return: queue of strings corresponding to a vertex, forming a path from
 *         start to target
 **/
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	a_star_node_t *node = NULL, **seen = NULL;
	heap_t *heap = NULL, *edge_heap = NULL;
	queue_t *queue = NULL;
	size_t i;

	if (!graph || !start || !target)
		return (NULL);

	if
	(
		(edge_heap = heap_init(graph->nb_vertices, edge_compare)) &&
		(seen      = calloc(graph->nb_vertices, sizeof(a_star_node_t *))) &&
		(heap      = heap_init(graph->nb_vertices, a_star_compare)) &&
		(seen[start->index] = a_star_node_init(start, NULL, 0, target))
	)
	{
		heap_push(seen[start->index], heap);

		while ((node = heap_pop(heap)))
		{
			printf("Checking %s, distance to %s is %d\n",
				(char *)node->vertex->content,
				(char *)target->content, (int)node->distance);

			if (node->vertex == target)
			{
				queue = make_result(node);
				break;
			}
			if (eval_neighbors(node, edge_heap, seen, heap, target) == -1)
				break;
		}
	}

	for (i = 0; i < graph->nb_vertices; i++)
		free(seen[i]);
	free(seen);
	heap_delete(edge_heap);
	heap_delete(heap);
	return (queue);
}

/**
 * eval_neighbors - evaluates the neighbors of a graph vertex
 * @node: node
 * @edge_heap: edge_heap heap
 * @seen: seen array
 * @heap: vertex heap
 * @target: target position
 * Return: 1 on failure | 0 on success
 **/
static int eval_neighbors(a_star_node_t *node, heap_t *edge_heap,
		a_star_node_t **seen, heap_t *heap, const vertex_t *target)
{
	edge_t *edge;
	vertex_t *vertex;
	int weight;

	/* Sort edge_heap */
	for (edge = node->vertex->edges; edge; edge = edge->next)
		heap_push(edge, edge_heap);

	/* For each edge, check its destination vertex */
	for (edge = heap_pop(edge_heap); edge; edge = heap_pop(edge_heap))
	{

		vertex = edge->dest;
		weight = edge->weight + node->weight;
		/**
		 * if vertex has not been seen, add to seen and vertex heap.
		 * else if path weight to vertex via this edge is shorter than the
		 * known path/weight to vertex, update seen[vertex->index].
		 */
		if (!seen[vertex->index])
		{
			if (!(seen[vertex->index] = a_star_node_init(vertex, node, weight, target)))
				return (-1);
			heap_push(seen[vertex->index], heap);
		}
		else if (weight < seen[vertex->index]->weight)
		{

			seen[vertex->index]->via = node;
			seen[vertex->index]->weight = weight;
			if (!in_heap(heap, seen[vertex->index]))
				heap_push(seen[vertex->index], heap);
			else
				heap_sort(heap);
		}
	}

	return (0);
}

/**
 * make_result - makes result
 * @node: a_start node
 * Return: queue of city names from start to dest
 */
static queue_t *make_result(a_star_node_t *node)
{
	queue_t *queue;
	char *s;

	if (!node)
		return (NULL);
	queue = queue_create();
	if (!queue)
		return (NULL);
	while (node)
	{
		s = strdup(node->vertex->content);
		if (!s)
		{
			queue_delete(queue);
			return (NULL);
		}
		queue_push_front(queue, s);
		node = node->via;
	}

	return (queue);
}

/**
 * a_star_compare - comparison function for heap of a_star_node_t objects
 * @a: first node
 * @b: second node
 * Return: 1 if a is greater than b | 0 otherwise
 **/
int a_star_compare(void *a, void *b)
{
	return
	(
		((a_star_node_t *)a)->weight + ((a_star_node_t *)a)->distance
			>
		((a_star_node_t *)b)->weight + ((a_star_node_t *)b)->distance
	);
}


