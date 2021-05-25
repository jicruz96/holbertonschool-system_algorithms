#include "pathfinding.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static queue_t *backtrack(graph_t *graph, char *seen, vertex_t const *curr,
			vertex_t const *target, queue_t *path);
static queue_t *add_city(char *city, queue_t *queue);

/**
 * backtracking_graph - searches for the first path from a starting point to a
 *                      target point in a graph.
 * @graph: pointer to the graph to go through
 * @start: pointer to the starting vertex
 * @target:pointer to the target vertex
 * Return: queue of strings corresponding to a vertex, forming a path from
 *         start to target
 */
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	queue_t *path = NULL;
	char *seen = NULL;

	if
	(
		!graph || !start || !target || !(path = queue_create()) ||
		!(seen = calloc(graph->nb_vertices, sizeof(char))) ||
		!backtrack(graph, seen, start, target, path)
	)
	{
		queue_delete(path);
		free(seen);
		return (NULL);
	}

	free(seen);
	return (add_city(strdup(start->content), path));
}

/**
 * backtrack - helper backtracking algorithm
 *
 * @graph: pointer to graph
 * @seen: pointer to array that keeps track of seen cells
 * @curr: current position in graph
 * @target: target position
 * @path: pointer to queue where final path will be stored.
 * Return: pointer to path
 */
static queue_t *backtrack(graph_t *graph, char *seen, vertex_t const *curr,
			vertex_t const *target, queue_t *path)
{
	edge_t *edge;

	if (!curr || seen[curr->index])
		return (NULL);

	seen[curr->index] = 1;

	printf("Checking %s\n", (char *)curr->content);

	if (curr == target)
		return (path);


	for (edge = curr->edges; edge; edge = edge->next)
		if (backtrack(graph, seen, edge->dest, target, path))
			return (add_city(strdup(edge->dest->content), path));

	return (NULL);
}

/**
 * add_city - adds a city to a queue
 *
 * @city: city name (string)
 * @queue: queue to place city in
 * Return: pointer to queue or NULL if city is NULL
 */
static queue_t *add_city(char *city, queue_t *queue)
{
	if (!city)
	{
		queue_delete(queue);
		return (NULL);
	}
	queue_push_front(queue, city);
	return (queue);
}
