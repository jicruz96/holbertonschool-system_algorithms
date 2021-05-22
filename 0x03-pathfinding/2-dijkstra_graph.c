#include "pathfinding.h"
#include "dk.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	dk_node_t **neighbors, **heap, *dk_nodes, *edges, *node, *tmp;
	size_t i, heap_size = 0, neighbors_size = 0;
	queue_t *queue;
	const edge_t *edge;

	if (!graph || !start || !target)
		return (NULL);
	
	dk_nodes  = calloc(graph->nb_vertices, sizeof(dk_node_t));
	edges     = calloc(graph->nb_vertices, sizeof(dk_node_t));
	heap      = calloc(graph->nb_vertices, sizeof(dk_node_t *));
	neighbors = calloc(graph->nb_vertices, sizeof(dk_node_t *));
	queue     = NULL;
	
	if (!dk_nodes || !heap)
		goto CLEANUP;
	
	node = dk_node_init(&dk_nodes[start->index], start, NULL, NULL, 0);
	
	while (node)
	{
		printf("Checking %s, distance from %s is %d\n",
				(char *)node->self.vertex->content, 
				(char *)start->content,
				node->weight);
		
		if (node->self.vertex == target)
		{
			queue = make_result(node);
			break;
		}
		
		for (i = 0, edge = node->self.vertex->edges; i < node->self.vertex->nb_edges; edge = edge->next, i++)
		{
			dk_node_init(&edges[i], NULL, edge, node, edge->weight);
			dk_heap_push(&edges[i], neighbors, &neighbors_size);
			
		}
		tmp = dk_heap_pop(neighbors, &neighbors_size);
		if (tmp)
		{
			for (edge = tmp->self.edge; edge; edge = tmp->self.edge)
			{
				tmp = &dk_nodes[edge->dest->index];
				if (tmp->self.vertex == NULL || tmp->weight > edge->weight + node->weight)
				{
					dk_node_init(tmp, edge->dest, NULL, node, edge->weight + node->weight);
					dk_heap_push(tmp, heap, &heap_size);
				}
				tmp = dk_heap_pop(neighbors, &neighbors_size);
				if (!tmp)
					break;
				
			}
		}
		memset(edges, 0, sizeof(dk_node_t) * graph->nb_vertices);
		node = dk_heap_pop(heap, &heap_size);
	}

CLEANUP:
	free(dk_nodes);
	free(heap);
	free(neighbors);
	free(edges);
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
		s = strdup(node->self.vertex->content);
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
