#include "pathfinding.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dk.c"
#include "edge.c"

static void print_msg(dk_node_t *node, const vertex_t *start);
static queue_t *make_result(dk_node_t *node);
static int eval_neighbors(dk_node_t *node, edge_t **edges,
		dk_node_t **seen, dk_node_t **dk_heap);


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
	dk_node_t *node = NULL, **seen = NULL, **dk_heap = NULL;
	edge_t **edges = NULL;
	queue_t *queue = NULL;
	size_t i;

	if (!graph || !start || !target)
		return (NULL);

	if
	(
		(edges   = calloc(graph->nb_vertices, sizeof(edge_t *))) &&
		(seen    = calloc(graph->nb_vertices, sizeof(dk_node_t *))) &&
		(dk_heap = calloc(graph->nb_vertices, sizeof(dk_node_t *))) &&
		(seen[start->index] = dk_node_init(start, NULL, 0))
	)
	{
		dk_heap_push(seen[start->index], dk_heap);

		while ((node = dk_heap_pop(dk_heap)))
		{
			print_msg(node, start);

			if (node->vertex == target)
			{
				queue = make_result(node);
				break;
			}
			if (eval_neighbors(node, edges, seen, dk_heap) == -1)
				break;
		}
	}

	for (i = 0; i < graph->nb_vertices; i++)
		free(seen[i]);
	free(seen);
	free(edges);
	free(dk_heap);
	return (queue);
}

/**
 * eval_neighbors - evaluates the neighbors of a graph vertex
 * @node: node
 * @edges: edges heap
 * @seen: seen array
 * @dk_heap: vertex heap
 * Return: 1 on failure | 0 on success
 **/
static int eval_neighbors(dk_node_t *node, edge_t **edges,
		dk_node_t **seen, dk_node_t **dk_heap)
{
	edge_t *edge;
	vertex_t *vertex;
	int weight;

	/* Sort edges */
	for (edge = node->vertex->edges; edge; edge = edge->next)
		edge_heap_push(edge, edges);

	/* For each edge, check destination */
	for (edge = edge_heap_pop(edges); edge; edge = edge_heap_pop(edges))
	{

		vertex = edge->dest, weight = edge->weight + node->weight;
		/**
		 * if vertex has not been seen, add to seen and vertex heap
		 * if this edge's path weight to this vertex is shorter than the
		 * known path/weight to the vertex, update the dk_vertex
		 */
		if (!seen[vertex->index])
		{
			if (!(seen[vertex->index] = dk_node_init(vertex, node, weight)))
				return (-1);
			dk_heap_push(seen[vertex->index], dk_heap);
		}
		else if (weight < seen[vertex->index]->weight)
		{

			seen[vertex->index]->via = node;
			seen[vertex->index]->weight = weight;
			dk_heap_push(seen[vertex->index], dk_heap);
		}
	}

	return (0);
}

/**
 * print_msg - prints current position and distance from start
 * @node: pointer to dk_node
 * @start: pointer to starting position
 **/
static void print_msg(dk_node_t *node, const vertex_t *start)
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
static queue_t *make_result(dk_node_t *node)
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
