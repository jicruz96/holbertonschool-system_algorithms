#include <stdio.h>
#include "dk.h"

/**
 * dk_heap_pop - pops top item off a dijkstra heap
 * @heap: pointer to heap
 * @size: size of heap
 * Return: popped node
 */
dk_node_t *dk_heap_pop(dk_node_t **heap, size_t *size)
{
	dk_node_t *tmp;
	size_t i;

	if (!heap || !(*heap) || !size || !(*size))
		return (NULL);

	tmp = heap[0];
	*size -= 1;

	for (i = 0; i < *size; i++)
		heap[i] = heap[i + 1];

	heap[*size] = NULL;
	return (tmp);
}

/**
 * dk_heap_push - pushes item onto a dijkstra heap
 * @node: pointer to node to add to heap
 * @heap: pointer to heap
 * @size: size of heap
 */
void dk_heap_push(dk_node_t *node, dk_node_t **heap, size_t *size)
{
	if (!node || !heap)
		return;
	heap[*size] = node;
	*size += 1;
	dk_heap_sort(heap, *size);
}


/**
 * dk_heap_sort - sorts array of dijkstra nodes using heap sort algorithm
 * @heap: pointer to array of pointers to dijkstra nodes to be sorted by weight
 * @size: size of heap
 **/
void dk_heap_sort(dk_node_t **heap, size_t size)
{
	int i;
	dk_node_t *tmp;

	if (!heap || !(*heap) || !size)
		return;
	for (i = (size / 2) - 1; i >= 0; i--)
		dk_heapify(heap, i, size);

	for (i = size - 1; i > 0; i--)
	{
		tmp = heap[0];
		heap[0] = heap[i];
		heap[i] = tmp;
		dk_heapify(heap, 0, i);
	}
}

/**
 * dk_heapify - turn an array of dijkstra nodes into a min heap
 * @heap: heap
 * @i: current index in heap to inspect
 * @size: size of heap
 **/
void dk_heapify(dk_node_t **heap, size_t i, size_t size)
{
	#define LEFT(x) ((2 * (x)) + 1)
	#define RIGHT(x) ((2 * (x)) + 2)
	size_t smallest = i;
	dk_node_t *tmp;


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
		dk_heapify(heap, smallest, size);
	}
}

/**
 * dk_node_init - initializes a dk_node_t data structure
 * @node: pointer to node to initialize
 * @curr: current position in graph (vertex_t)
 * @via: previous postion in graph (dk_node_t)
 * @weight: weight of path from start to curr via prev
 * Return: pointer to new node
 */
dk_node_t *dk_node_init(dk_node_t *node, const vertex_t *vertex, 
		const edge_t *edge, dk_node_t *via, int weight)
{
	if (node)
	{
		if (vertex)
			node->self.vertex = vertex;
		else
			node->self.edge = edge;
		node->via = via;
		node->weight = weight;
	}

	return (node);
}
