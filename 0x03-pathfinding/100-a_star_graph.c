#include "pathfinding.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "a.c"
#include "edge.c"

static void print_msg(a_node_t *node, const vertex_t *start);
static queue_t *make_result(a_node_t *node);
static int eval_neighbors(a_node_t *node, edge_t **edges,
		a_node_t **seen, a_node_t **a_heap, const vertex_t *target);


/**
 * a_star_graph - searches for the shortest and fastest path from a starting
 *                point to a target point in a graph.
 * @graph: pointer to the graph to go through
 * @start: pointer to the starting vertex
 * @target: pointer to the target vertex
 * Return: queue of strings corresponding to a vertex, forming a path from
 *         start to target
 **/
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	a_node_t *node = NULL, **seen = NULL, **a_heap = NULL;
	edge_t **edges = NULL;
	queue_t *queue = NULL;
	size_t i;

	if (!graph || !start || !target)
		return (NULL);

	if
	(
		(edges   = calloc(graph->nb_vertices, sizeof(edge_t *))) &&
		(seen    = calloc(graph->nb_vertices, sizeof(a_node_t *))) &&
		(a_heap = calloc(graph->nb_vertices, sizeof(a_node_t *))) &&
		(seen[start->index] = a_node_init(start, NULL, 0, target))
	)
	{
		a_heap_push(seen[start->index], a_heap);

		while ((node = a_heap_pop(a_heap)))
		{
			print_msg(node, start);

			if (node->vertex == target)
			{
				queue = make_result(node);
				break;
			}
			if (eval_neighbors(node, edges, seen, a_heap, target) == -1)
				break;
		}
	}

	for (i = 0; i < graph->nb_vertices; i++)
		free(seen[i]);
	free(seen);
	free(edges);
	free(a_heap);
	return (queue);
}

/**
 * eval_neighbors - evaluates the neighbors of a graph vertex
 * @node: node
 * @edges: edges heap
 * @seen: seen array
 * @a_heap: vertex heap
 * @target: target
 * Return: 1 on failure | 0 on success
 **/
static int eval_neighbors(a_node_t *node, edge_t **edges,
		a_node_t **seen, a_node_t **a_heap, const vertex_t *target)
{
	edge_t *edge;
	vertex_t *vertex;
	int weight;

	for (edge = node->vertex->edges; edge; edge = edge->next)
		edge_heap_push(edge, edges);


	for (edge = edge_heap_pop(edges); edge; edge = edge_heap_pop(edges))
	{

		vertex = edge->dest, weight = edge->weight + node->weight;

		if (!seen[vertex->index])
		{
			if (!(seen[vertex->index] = a_node_init(vertex, node, weight, target)))
				return (-1);
			a_heap_push(seen[vertex->index], a_heap);
		}
		else if (weight < seen[vertex->index]->weight)
		{
			seen[vertex->index]->via = node;
			seen[vertex->index]->weight = weight;
			a_heap_push(seen[vertex->index], a_heap);
		}
	}

	return (0);
}

/**
 * print_msg - prints current position and distance from start
 * @node: pointer to a_node
 * @start: pointer to starting position
 **/
static void print_msg(a_node_t *node, const vertex_t *start)
{
	if (node && start)
	{
		printf("Checking %s, distance from %s is %d\n",
				(char *)node->vertex->content,
				(char *)start->content,
				node->distance);
	}
}

/**
 * make_result - makes result
 * @node: dijkstra node
 * Return: queue of city names from start to dest
 */
static queue_t *make_result(a_node_t *node)
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
