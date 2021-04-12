#include "graphs.h"
#include <stdlib.h>
#include <string.h>

/**
 * vertex_init - allocates memory for a new vertex
 * @str: content to be stroed in vertex
 * @index: vertex index number
 * Return: pointer to new vertex
 **/
static vertex_t *vertex_init(const char *str, size_t index)
{
	vertex_t *new_vertex = malloc(sizeof(vertex_t));

	if (new_vertex)
	{
		new_vertex->content = strdup(str);
		new_vertex->nb_edges = 0;
		new_vertex->edges = NULL;
		new_vertex->next = NULL;
		new_vertex->index = index;
	}

	return (new_vertex);
}

/**
 * graph_add_vertex - adds a vertex to a graph
 * @graph: pointer to graph object
 * @str: content of new vertex
 * Return: pointer to new vertex
 **/
vertex_t *graph_add_vertex(graph_t *graph, const char *str)
{
	vertex_t **p;

	if (graph == NULL || str == NULL)
		return (NULL);

	for (p = &graph->vertices; *p != NULL; p = &(*p)->next)
		if (strcmp(str, (*p)->content) == 0)
			return (NULL);

	*p = vertex_init(str, graph->nb_vertices);
	graph->nb_vertices += 1;
	return (*p);
}
