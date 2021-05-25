#include "pathfinding.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "heap.h"
#include "heap.c"
#include "a_star.c"
#include "a_star.h"

static void print_msg(a_star_node_t *node, const vertex_t *start);
static queue_t *make_result(a_star_node_t *node);
static int eval_neighbors(a_star_node_t *node, heap_t *edge_heap,
		a_star_node_t **seen, heap_t *heap, const vertex_t *target);
static int a_star_compare(void *vertex1, void *vertex2);
static int edge_compare(void *edge1, void *edge2);


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
			print_msg(node, target);

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
 * edge_compare - comparison function for heap of edge_t objects
 * @a: first edge
 * @b: second edge
 * Return: 1 if a is greater than b | 0 otherwise
 **/
int edge_compare(void *a, void *b)
{
	return (((edge_t *)a)->weight > ((edge_t *)b)->weight);
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



/**
 * distance_to - returns distance from curr to target
 *
 * @curr: current position vertex
 * @target: target vertex
 * Return: distance
 */
double distance_to(const vertex_t *curr, const vertex_t *target)
{
	int x = curr->x - target->x;
	int y = curr->y - target->y;

	return (sqrt(x * x + y * y));
}



/**
 * print_msg - prints current position and distance to target
 * @node: pointer to a_star_node
 * @target: pointer to target position
 **/
static void print_msg(a_star_node_t *node, const vertex_t *target)
{
	if (node && target)
	{
		printf("Checking %s, distance to %s is %d\n",
				(char *)node->vertex->content,
				(char *)target->content,
				(int)node->distance);
	}
}
