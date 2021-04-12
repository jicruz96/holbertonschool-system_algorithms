#include "graphs.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * helper - helper function for depth_first_traverse
 * @v: vertex being dealt with
 * @action: action to take on vertex
 * @seen: array that keeps track of which vertexes have been seen
 * @depth: current depth of traversal
 * Return: maximum depth encountered
 **/
size_t helper(
	const vertex_t *v, action_t action, char *seen, size_t depth)
{
	edge_t *e;
	size_t tmp, res = 0;

	if (seen[v->index])
		return (depth - 1);

	action(v, depth);
	seen[v->index] = 1;
	for (e = v->edges; e; e = e->next)
	{
		tmp = helper(e->dest, action, seen, depth + 1);
		res = tmp > res ? tmp : res;
	}

	return (res);
}

/**
 * depth_first_traverse - traverses a graph using depth-first algorithm
 * @graph: pointer to graph
 * @action: pointer to function to perform on graph
 * Return: the biggest vertex depth in the graph | or 0 on failure
 **/
size_t depth_first_traverse(
	const graph_t *graph, void (*action)(const vertex_t *v, size_t depth)
)
{
	char *seen;
	size_t max_depth = 0;

	if (graph == NULL)
		return (0);

	if (graph->nb_vertices)
	{
		seen = calloc(graph->nb_vertices, sizeof(char));
		max_depth = helper(graph->vertices, action, seen, max_depth);
		free(seen);
	}
	return (max_depth);

}
