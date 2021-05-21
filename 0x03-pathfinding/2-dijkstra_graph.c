#include "pathfinding.h"
#include "dk.c"
#include <stdlib.h>

static queue_t *make_result(dk_node_t *node);

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
	dk_node_t **heap, *dk_nodes, *node, *tmp;
	queue_t *queue;
	edge_t *edge;
	size_t size;

	if (!graph || !start || !target)
		return (NULL);
	size     = 0;
	dk_nodes = calloc(graph->nb_vertices, sizeof(dk_node_t));
	heap     = calloc(graph->nb_vertices, sizeof(dk_node_t *));
	queue    = NULL;
	if (!dk_nodes || !heap)
		goto CLEANUP;
	dk_node_init(&dk_nodes[start->index], start, NULL, 0);
	for (node = &dk_nodes[start->index]; node; node = dk_heap_pop(heap, &size))
	{
		if (node->self == target)
		{
			queue = make_result(node);
			break;
		}
		for (edge = node->self->edges; edge; edge = edge->next)
		{
			tmp = &dk_nodes[edge->dest->index];
			if (tmp->self == NULL)
			{
				dk_node_init(tmp, edge->dest, node, edge->weight);
				dk_heap_push(tmp, heap, &size);
			}
			else if (tmp->weight > edge->weight + node->weight)
			{
				tmp->via = node, tmp->weight = edge->weight + node->weight;
				dk_heap_push(tmp, heap, &size);
			}
		}
	}
CLEANUP:
	free(dk_nodes);
	free(heap);
	return (queue);
}

/**
 * make_result - makes result
 * @node: dijkstra node
 * Return: queue of city names from start to dest
 */
queue_t *make_result(dk_node_t *node)
{
	queue_t *queue;
	char *s;

	queue = queue_create();
	if (!queue)
		return (NULL);
	while (node)
	{
		s = strdup(node->self->content);
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
