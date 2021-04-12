#include "graphs.h"
#include <stdlib.h>

/**
 * graph_delete - deletes a graph object (frees all objects from heap)
 * @graph: pointer to graph
 **/
void graph_delete(graph_t *graph)
{
	vertex_t *v;
	edge_t *e;
	void *tmp;

	if (graph == NULL)
		return;

	v = graph->vertices;
	while (v)
	{
		free(v->content);
		e = v->edges;
		while (e)
		{
			tmp = (void *)e;
			e = e->next;
			free(tmp);
		}
		tmp = (void *)v;
		v = v->next;
		free(tmp);
	}

	free(graph);
}
