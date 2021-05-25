#include <stdio.h>
#include "a.h"

/**
 * a_heap_pop - pops top item off a dijkstra heap
 * @heap: pointer to heap
 * Return: popped node
 */
a_node_t *a_heap_pop(a_node_t **heap)
{
	a_node_t *tmp;
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
 * a_heap_push - pushes item onto a dijkstra heap
 * @node: pointer to node to add to heap
 * @heap: pointer to heap
 */
void a_heap_push(a_node_t *node, a_node_t **heap)
{
	size_t i;

	if (!node || !heap)
		return;
	for (i = 0; heap[i]; i++)
		if (node == heap[i])
		{
			while (heap[i])
				i++;
			a_heap_sort(heap, i);
			return;
		}

	heap[i] = node;
	a_heap_sort(heap, i + 1);
}


/**
 * a_heap_sort - sorts array of dijkstra nodes using heap sort algorithm
 * @heap: pointer to array of pointers to dijkstra nodes to be sorted by weight
 * @size: size of heap
 **/
void a_heap_sort(a_node_t **heap, size_t size)
{
	int i;
	a_node_t *tmp;

	if (!heap || !(*heap) || !size)
		return;

	for (i = (size / 2) - 1; i >= 0; i--)
		a_heapify(heap, i, size);

	for (i = size - 1; i > 0; i--)
	{
		tmp = heap[0];
		heap[0] = heap[i];
		heap[i] = tmp;
		a_heapify(heap, 0, i);
	}
}

/**
 * a_heapify - turn an array of dijkstra nodes into a min heap
 * @heap: heap
 * @i: current index in heap to inspect
 * @size: size of heap
 **/
void a_heapify(a_node_t **heap, size_t i, size_t size)
{
	#define LEFT(x) ((2 * (x)) + 1)
	#define RIGHT(x) ((2 * (x)) + 2)
	#define dist(node) (node->weight + node->distance)
	size_t smallest = i;
	a_node_t *tmp;


	if (LEFT(i) < size && dist(heap[LEFT(i)]) >= dist(heap[i]))
	{
		if (RIGHT(i) < size && dist(heap[RIGHT(i)]) > dist(heap[LEFT(i)]))
			smallest = RIGHT(i);
		else
			smallest = LEFT(i);
	}
	else if (RIGHT(i) < size && dist(heap[RIGHT(i)]) >= dist(heap[i]))
	{
		smallest = RIGHT(i);
	}

	if (smallest != i)
	{
		tmp = heap[smallest];
		heap[smallest] = heap[i];
		heap[i] = tmp;
		a_heapify(heap, smallest, size);
	}
}

/**
 * a_node_init - initializes a a_node_t data structure
 * @vertex: current position in graph (vertex_t)
 * @via: previous postion in graph (a_node_t)
 * @weight: weight of path from start to curr via prev
 * @target: pointer to target node (used to calculate distance)
 * Return: pointer to new node
 */
a_node_t *a_node_init(const vertex_t *vertex, a_node_t *via, int weight,
	const vertex_t *target)
{
	a_node_t *node = malloc(sizeof(a_node_t));

	if (node)
	{
		node->vertex = vertex;
		node->via = via;
		node->weight = weight;
		node->distance = distance_to(vertex, target);
	}

	return (node);
}
