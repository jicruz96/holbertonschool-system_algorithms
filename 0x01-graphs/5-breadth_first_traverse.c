#include "graphs.h"
#include <stdlib.h>


/**
 * breadth_first_traverse - traverse a graph using breadth-first algorithm
 * @graph: pointer to graph
 * @action: pointer to function that will act on each vertex traversed
 * Return: biggest vertex depth
 **/
size_t breadth_first_traverse(
	const graph_t *graph, void (*action)(const vertex_t *v, size_t depth))
{
	size_t depth = 0, round_size = 0;
	vertex_t **vertices;
	char *seen;
	edge_t *p;
	int i, j;


	if (!graph || !action || graph->nb_vertices == 0)
		return (0);

	seen = calloc(graph->nb_vertices, sizeof(char));
	vertices = calloc(graph->nb_vertices + 1, sizeof(vertex_t *));
	vertices[0] = graph->vertices;
	seen[0] = 1;

	for (i = 0, j = 1, round_size = 1; vertices[i]; i++)
	{
		action(vertices[i], depth);

		if (--round_size == 0)
		{
			depth++;
			round_size = vertices[i]->nb_edges;
		}

		for (p = vertices[i]->edges; p; p = p->next)
		{
			if (seen[p->dest->index] == 0)
			{
				vertices[j++] = p->dest;
				seen[p->dest->index] = 1;
			}
		}
	}

	free(seen);
	free(vertices);

	return (depth - 1);
}
