#include <stdio.h>
#include "edge_heap.h"

/**
 * edge_heap_pop - pops top item off a dijkstra heap
 * @heap: pointer to heap
 * Return: popped node
 */
edge_t *edge_heap_pop(edge_t **heap)
{
	edge_t *tmp;
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
 * edge_heap_push - pushes item onto a dijkstra heap
 * @node: pointer to node to add to heap
 * @heap: pointer to heap
 */
void edge_heap_push(edge_t *node, edge_t **heap)
{
	size_t i;

	if (!node || !heap)
		return;

	for (i = 0; heap[i]; i++)
		;

	heap[i] = node;
	edge_heap_sort(heap, i + 1);
}


/**
 * edge_heap_sort - sorts array of dijkstra nodes using heap sort algorithm
 * @heap: pointer to array of pointers to dijkstra nodes to be sorted by weight
 * @size: size of heap
 **/
void edge_heap_sort(edge_t **heap, size_t size)
{
	int i;
	edge_t *tmp;

	if (!heap || !(*heap) || !size)
		return;
	for (i = (size / 2) - 1; i >= 0; i--)
		edge_heapify(heap, i, size);

	for (i = size - 1; i > 0; i--)
	{
		tmp = heap[0];
		heap[0] = heap[i];
		heap[i] = tmp;
		edge_heapify(heap, 0, i);
	}
}

/**
 * edge_heapify - turn an array of dijkstra nodes into a min heap
 * @heap: heap
 * @i: current index in heap to inspect
 * @size: size of heap
 **/
void edge_heapify(edge_t **heap, size_t i, size_t size)
{
	#define LEFT(x) ((2 * (x)) + 1)
	#define RIGHT(x) ((2 * (x)) + 2)
	size_t smallest = i;
	edge_t *tmp;


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
		edge_heapify(heap, smallest, size);
	}
}
