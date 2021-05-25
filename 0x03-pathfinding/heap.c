#include <stdlib.h>
#include "heap.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * heap_pop - pops top item off a dijkstra heap
 * @heap: pointer to heap
 * Return: popped node
 */
void *heap_pop(heap_t *heap)
{
	void *tmp;
	size_t i;

	if (!heap || heap->data_size == 0)
		return (NULL);
	
	tmp = heap->data[0];
	heap->data_size -= 1;

	for (i = 0; i < heap->data_size; i++)
		heap->data[i] = heap->data[i + 1];

	heap->data[heap->data_size] = NULL;

	return (tmp);
}

/**
 * heap_push - pushes item onto a dijkstra heap
 * @node: pointer to node to add to heap
 * @heap: pointer to heap
 * Return: pointer to node within heap or NULL if node or heap is NULL
 */
void *heap_push(void *node, heap_t *heap)
{
	if (!node || !heap)
		return (NULL);
	
	if (heap->data_size + 1 == heap->size)
	{
		heap->size = (size_t)max(heap->size * 1.1, heap->size + 1);
		heap->data = realloc(heap->data, heap->size * sizeof (void *));
	}
	
	heap->data[heap->data_size] = node;
	heap->data_size += 1;
	heap_sort(heap);
	return (node);
}


/**
 * heap_sort - sorts array of dijkstra nodes using heap sort algorithm
 * @heap: pointer to array of pointers to dijkstra nodes to be sorted by weight
 * @size: size of heap
 **/
void heap_sort(heap_t *heap)
{
	int i;
	void *tmp;

	if (!heap || !heap->data_size)
		return;

	for (i = (heap->data_size / 2) - 1; i >= 0; i--)
		heapify(heap, i, heap->data_size);

	for (i = heap->data_size - 1; i > 0; i--)
	{
		tmp = heap->data[0];
		heap->data[0] = heap->data[i];
		heap->data[i] = tmp;
		heapify(heap, 0, i);
	}
}

/**
 * heapify - turn an array of dijkstra nodes into a min heap
 * @heap: heap
 * @i: current index in heap to inspect
 * @size: size of heap
 **/
void heapify(heap_t *heap, size_t i, size_t size)
{
	#define LEFT(x) ((2 * (x)) + 1)
	#define RIGHT(x) ((2 * (x)) + 2)
	int (*compare)(void *, void *) = heap->compare;
	void *tmp, **data = heap->data;
	size_t smallest = i;


	if (LEFT(i) < size && compare(data[LEFT(i)], data[i]))
	{
		if (RIGHT(i) < size && compare(data[RIGHT(i)], data[LEFT(i)]))
			smallest = RIGHT(i);
		else
			smallest = LEFT(i);
	}
	else if (RIGHT(i) < size && compare(data[RIGHT(i)], data[i]))
	{
		smallest = RIGHT(i);
	}

	if (smallest != i)
	{
		tmp = data[smallest];
		data[smallest] = data[i];
		data[i] = tmp;
		heapify(heap, smallest, size);
	}
}

/**
 * node_init - initializes a void data structure
 * @vertex: current position in graph (vertex_t)
 * @via: previous postion in graph (void)
 * @weight: weight of path from start to curr via prev
 * Return: pointer to new node
 */
heap_t *heap_init(size_t size, int (*compare)(void *, void *))
{
	heap_t *heap = malloc(sizeof(heap_t));

	if (heap)
	{
		if (!(heap->data = calloc(size, sizeof(void *))))
		{
			free(heap);
			return (NULL);
		}
		heap->compare = compare;
		heap->data_size = 0;
		heap->size = size;
	}

	return (heap);
}

/**
 * in_heap - checks if a piece of data exists inside a heap
 * 
 * @heap: pointer to heap 
 * @datum: pointer to datum 
 * Return: 1 if datum in heap | 0 if not 
 */
int in_heap(heap_t *heap, void *datum)
{
	size_t i;

	if (!heap || !datum)
		return (0);

	for (i = 0; i < heap->data_size; i++)
		if (heap->data[i] == datum)
			return (1);
	
	return (0);
}

/**
 * heap_delete - deletes a heap object
 * @heap: pointer to heap
 */
void heap_delete(heap_t *heap)
{
	if (heap)
	{
		free(heap->data);
		free(heap);
	}
}
