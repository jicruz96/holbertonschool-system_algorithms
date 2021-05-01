#include "huffman.h"
#include <stdlib.h>

#include <stdio.h>
/**
 * huffman_priority_queue - creates priority queue for Huffman coding algorithm
 * @data: array of characters
 * @freq: array of frequencies of each character in data
 * @size: sizes of the arrays
 * Return: pointer to the priority queue
 **/
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size)
{
	size_t i;
	symbol_t *symbol;
	binary_tree_node_t *node;
	heap_t *heap;

	if (data == NULL || freq == NULL || size == 0)
		return (NULL);

	heap = heap_create(&compare_frequencies);

	if (heap == NULL)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		symbol = symbol_create(data[i], freq[i]);
		if (symbol == NULL)
		{
			heap_delete(heap, free_data);
			return (NULL);
		}
		node = binary_tree_node(NULL, (void *)symbol);
		if (node == NULL)
		{
			heap_delete(heap, free_data);
			return (NULL);
		}
		heap_insert(heap, node);
	}
	return (heap);
}

/**
 * compare_frequencies - compares the frequency values of two nodes in a
 *                       huffman priority queue.
 * @node1: node 1
 * @node2: node 2
 * Return: 1 if f1 is greater, -1 if f2 is greater, 0 if equivalent
 **/
int compare_frequencies(void *node1, void *node2)
{
	symbol_t *symbol1 = (symbol_t *)((binary_tree_node_t *)node1)->data;
	symbol_t *symbol2 = (symbol_t *)((binary_tree_node_t *)node2)->data;

	if (symbol1->freq > symbol2->freq)
		return (1);

	if (symbol1->freq > symbol2->freq)
		return (-1);

	return (0);
}

/**
 * free_data - frees internal data of a node
 * @node: node
 **/
void free_data(void *node)
{
	if (node == NULL)
		return;

	free(((binary_tree_node_t *)node)->data);
}
