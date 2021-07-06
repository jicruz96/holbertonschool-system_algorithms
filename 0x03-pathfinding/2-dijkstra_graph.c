#include "pathfinding.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.c"
#include "edge_compare.c"

static queue_t *make_result(dijkstra_node_t *node);
static int eval_neighbors(dijkstra_node_t *node, heap_t *edge_heap,
		dijkstra_node_t **seen, heap_t *heap);
static int dijkstra_compare(void *vertex1, void *vertex2);
static dijkstra_node_t *dijkstra_node_init(const vertex_t *vertex,
	dijkstra_node_t *via, int weight);




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
	dijkstra_node_t *node = NULL, **seen = NULL;
	heap_t *heap = NULL, *edge_heap = NULL;
	queue_t *queue = NULL;
	size_t i;

	if (!graph || !start || !target)
		return (NULL);

	if
	(
		(edge_heap = heap_init(graph->nb_vertices, edge_compare)) &&
		(seen      = calloc(graph->nb_vertices, sizeof(dijkstra_node_t *))) &&
		(heap      = heap_init(graph->nb_vertices, dijkstra_compare)) &&
		(seen[start->index] = dijkstra_node_init(start, NULL, 0))
	)
	{
		heap_push(seen[start->index], heap);

		while ((node = heap_pop(heap)))
		{
			printf("Checking %s, distance from %s is %d\n",
				(char *)node->vertex->content,
				(char *)start->content, node->weight);

			if (node->vertex == target)
			{
				queue = make_result(node);
				break;
			}
			if (eval_neighbors(node, edge_heap, seen, heap) == -1)
				break;
		}
	}

	for (i = 0; i < graph->nb_vertices; i++)
		free(seen[i]);
	free(seen);
	heap_delete(edge_heap);
	heap_delete(heap);
	return (queue);
}

/**
 * dijkstra_node_init - initializes a dijkstra_node_t data structure
 * @vertex: current position in graph (vertex_t)
 * @via: previous postion in graph (dijkstra_node_t)
 * @weight: weight of path from start to curr via prev
 * Return: pointer to new node
 */
static dijkstra_node_t *dijkstra_node_init(const vertex_t *vertex,
			dijkstra_node_t *via, int weight)
{
	dijkstra_node_t *node = malloc(sizeof(dijkstra_node_t));

	if (node)
	{
		node->vertex = vertex;
		node->via = via;
		node->weight = weight;
	}

	return (node);
}

/**
 * eval_neighbors - evaluates the neighbors of a graph vertex
 * @node: node
 * @edge_heap: edge_heap heap
 * @seen: seen array
 * @heap: vertex heap
 * Return: 1 on failure | 0 on success
 **/
static int eval_neighbors(dijkstra_node_t *node, heap_t *edge_heap,
		dijkstra_node_t **seen, heap_t *heap)
{
	edge_t *edge;
	vertex_t *vertex;
	int weight;

	/* Sort edge_heap */
	for (edge = node->vertex->edges; edge; edge = edge->next)
		heap_push(edge, edge_heap);

	/* For each edge, check its destination vertex */
	for (edge = heap_pop(edge_heap); edge; edge = heap_pop(edge_heap))
	{

		vertex = edge->dest;
		weight = edge->weight + node->weight;
		/**
		 * if vertex has not been seen, add to seen and vertex heap.
		 * else if path weight to vertex via this edge is shorter than the
		 * known path/weight to vertex, update seen[vertex->index].
		 */
		if (!seen[vertex->index])
		{
			if (!(seen[vertex->index] = dijkstra_node_init(vertex, node, weight)))
				return (-1);
			heap_push(seen[vertex->index], heap);
		}
		else if (weight < seen[vertex->index]->weight)
		{

			seen[vertex->index]->via = node;
			seen[vertex->index]->weight = weight;
			if (!in_heap(heap, seen[vertex->index]))
				heap_push(seen[vertex->index], heap);
			else
				heap_sort(heap);
		}
	}

	return (0);
}


/**
 * make_result - makes result
 * @node: dijkstra node
 * Return: queue of city names from start to dest
 */
static queue_t *make_result(dijkstra_node_t *node)
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


/**
 * dijkstra_compare - comparison function for heap of dijkstra_node_t objects
 * @a: first node
 * @b: second node
 * Return: 1 if a is greater than b | 0 otherwise
 **/
int dijkstra_compare(void *a, void *b)
{
	return (((dijkstra_node_t *)a)->weight > ((dijkstra_node_t *)b)->weight);
}
