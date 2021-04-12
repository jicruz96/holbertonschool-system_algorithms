#include "graphs.h"
#include <stdlib.h>

/**
 * graph_create - initialize a graph structure
 * Return: pointer to graph
 **/
graph_t *graph_create(void)
{
	graph_t *new = malloc(sizeof(graph_t));

	if (new)
	{
		new->nb_vertices = 0;
		new->vertices = NULL;
	}

	return (new);
}
