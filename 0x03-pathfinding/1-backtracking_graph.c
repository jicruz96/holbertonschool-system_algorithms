#include "pathfinding.h"
#include <string.h>

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
	queue_t *path;
	char *seen;

	if (!graph || !start || !target)
		return (NULL);

	path = queue_create();
	if (!path)
		return (NULL);
	seen = calloc(graph->nb_vertices, sizeof(char));
	if (!seen)
		return (NULL);

	backtrack(graph, seen, start, target, path);
	free(seen);
	return (path);
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
queue_t *backtrack(graph_t *graph, char *seen, vertex_t const *curr,
			vertex_t const *target, queue_t *path)
{
	edge_t *edge;
	char *city;

	if (!curr || seen[curr->index])
		return (NULL);

	seen[curr->index] = 1;

	printf("Checking %s\n", (char *)curr->content);

	for (edge = curr->edges; edge; edge = edge->next)
		if (
			edge->dest == target ||
			backtrack(graph, seen, edge->dest, target, path)
		)
		{
			city = strdup(edge->dest->content);
			if (!city)
			{
				queue_delete(path);
				return (NULL);
			}
			queue_push_front(path, city);
			return (path);
		}

	return (NULL);
}
