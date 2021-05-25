#include <stdio.h>
#include "dijkstra_heap.h"
#include "edge_heap.h"

/**
 * dijkstra_heap_pop - pops top item off a dijkstra heap
 * @heap: pointer to heap
 * Return: popped node
 */
dijkstra_node_t *dijkstra_heap_pop(dijkstra_node_t **heap)
{
	dijkstra_node_t *tmp;
	size_t i;

	if (!heap || !(*heap))
		return (NULL);
	tmp = heap[0];
	for (i = 0; heap[i + 1]; i++)
		heap[i] = heap[i + 1];
	heap[i] = NULL;

	return (tmp);
}

/**
 * dijkstra_heap_push - pushes item onto a dijkstra heap
 * @node: pointer to node to add to heap
 * @heap: pointer to heap
 */
void dijkstra_heap_push(dijkstra_node_t *node, dijkstra_node_t **heap)
{
	size_t i;

	if (!node || !heap)
		return;
	for (i = 0; heap[i]; i++)
		if (node == heap[i])
		{
			while (heap[i])
				i++;
			dijkstra_heap_sort(heap, i);
			return;
		}

	heap[i] = node;
	dijkstra_heap_sort(heap, i + 1);
}


/**
 * dijkstra_heap_sort - sorts array of dijkstra nodes using heap sort algorithm
 * @heap: pointer to array of pointers to dijkstra nodes to be sorted by weight
 * @size: size of heap
 **/
void dijkstra_heap_sort(dijkstra_node_t **heap, size_t size)
{
	int i;
	dijkstra_node_t *tmp;

	if (!heap || !(*heap) || !size)
		return;

	for (i = (size / 2) - 1; i >= 0; i--)
		dijkstra_heapify(heap, i, size);

	for (i = size - 1; i > 0; i--)
	{
		tmp = heap[0];
		heap[0] = heap[i];
		heap[i] = tmp;
		dijkstra_heapify(heap, 0, i);
	}
}

/**
 * dijkstra_heapify - turn an array of dijkstra nodes into a min heap
 * @heap: heap
 * @i: current index in heap to inspect
 * @size: size of heap
 **/
void dijkstra_heapify(dijkstra_node_t **heap, size_t i, size_t size)
{
	#define LEFT(x) ((2 * (x)) + 1)
	#define RIGHT(x) ((2 * (x)) + 2)
	size_t smallest = i;
	dijkstra_node_t *tmp;


	if (LEFT(i) < size && heap[LEFT(i)]->weight >= heap[i]->weight)
	{
		if (RIGHT(i) < size && heap[RIGHT(i)]->weight > heap[LEFT(i)]->weight)
			smallest = RIGHT(i);
		else
			smallest = LEFT(i);
	}
	else if (RIGHT(i) < size && heap[RIGHT(i)]->weight >= heap[i]->weight)
	{
		smallest = RIGHT(i);
	}

	if (smallest != i)
	{
		tmp = heap[smallest];
		heap[smallest] = heap[i];
		heap[i] = tmp;
		dijkstra_heapify(heap, smallest, size);
	}
}

/**
 * dijkstra_node_init - initializes a dijkstra_node_t data structure
 * @vertex: current position in graph (vertex_t)
 * @via: previous postion in graph (dijkstra_node_t)
 * @weight: weight of path from start to curr via prev
 * Return: pointer to new node
 */
dijkstra_node_t *dijkstra_node_init(const vertex_t *vertex, dijkstra_node_t *via, int weight)
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
