#include "pathfinding.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dijkstra_heap.c"
#include "edge_heap.c"

static void print_msg(dijkstra_node_t *node, const vertex_t *start);
static queue_t *make_result(dijkstra_node_t *node);
static int eval_neighbors(dijkstra_node_t *node, edge_t **edge_heap,
		dijkstra_node_t **seen, dijkstra_node_t **heap);


/**
 * dijkstra_graph - searches for the shortest path from a starting point to a
 *                  target point in a graph.
 * @graph: pointer to the graph to go through
 * @start: pointer to the starting vertex
 * @target: pointer to the target vertex
 * Return: queue of strings corresponding to a vertex, forming a path from
 *         start to target
 **/
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	dijkstra_node_t *node = NULL, **seen = NULL, **heap = NULL;
	edge_t **edge_heap = NULL;
	queue_t *queue = NULL;
	size_t i;

	if (!graph || !start || !target)
		return (NULL);

	if
	(
		(edge_heap = calloc(graph->nb_vertices, sizeof(edge_t *))) &&
		(seen      = calloc(graph->nb_vertices, sizeof(dijkstra_node_t *))) &&
		(heap      = calloc(graph->nb_vertices, sizeof(dijkstra_node_t *))) &&
		(seen[start->index] = dijkstra_node_init(start, NULL, 0))
	)
	{
		dijkstra_heap_push(seen[start->index], heap);

		while ((node = dijkstra_heap_pop(heap)))
		{
			print_msg(node, start);

			if (node->vertex == target)
			{
				queue = make_result(node);
				break;
			}
			if (eval_neighbors(node, edge_heap, seen, heap) == -1)
				break;
		}
	}

	for (i = 0; i < graph->nb_vertices; i++)
		free(seen[i]);
	free(seen);
	free(edge_heap);
	free(heap);
	return (queue);
}

/**
 * eval_neighbors - evaluates the neighbors of a graph vertex
 * @node: node
 * @edge_heap: edge_heap heap
 * @seen: seen array
 * @heap: vertex heap
 * Return: 1 on failure | 0 on success
 **/
static int eval_neighbors(dijkstra_node_t *node, edge_t **edge_heap,
		dijkstra_node_t **seen, dijkstra_node_t **heap)
{
	edge_t *edge;
	vertex_t *vertex;
	int weight;

	/* Sort edge_heap */
	for (edge = node->vertex->edges; edge; edge = edge->next)
		edge_heap_push(edge, edge_heap);

	/* For each edge, check its destination vertex */
	for (edge = edge_heap_pop(edge_heap); edge; edge = edge_heap_pop(edge_heap))
	{

		vertex = edge->dest, weight = edge->weight + node->weight;
		/**
		 * if vertex has not been seen, add to seen and vertex heap.
		 * else if path weight to vertex via this edge is shorter than the
		 * known path/weight to vertex, update seen[vertex->index].
		 */
		if (!seen[vertex->index])
		{
			if (!(seen[vertex->index] = dijkstra_node_init(vertex, node, weight)))
				return (-1);
			dijkstra_heap_push(seen[vertex->index], heap);
		}
		else if (weight < seen[vertex->index]->weight)
		{

			seen[vertex->index]->via = node;
			seen[vertex->index]->weight = weight;
			dijkstra_heap_push(seen[vertex->index], heap);
		}
	}

	return (0);
}

/**
 * print_msg - prints current position and distance from start
 * @node: pointer to dijkstra_node
 * @start: pointer to starting position
 **/
static void print_msg(dijkstra_node_t *node, const vertex_t *start)
{
	if (node && start)
	{
		printf("Checking %s, distance from %s is %d\n",
				(char *)node->vertex->content,
				(char *)start->content,
				node->weight);
	}
}

/**
 * make_result - makes result
 * @node: dijkstra node
 * Return: queue of city names from start to dest
 */
static queue_t *make_result(dijkstra_node_t *node)
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
