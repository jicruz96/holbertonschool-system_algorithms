#include "huffman.h"
#include <stdlib.h>

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
		symbol = i > 3 ? NULL : symbol_create(data[i], freq[i]);
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
 * @p1: pointer 1
 * @p2: pointer 2
 * Return: 1 if f1 is greater, -1 if f2 is greater, 0 if equivalent
 **/
int compare_frequencies(void *p1, void *p2)
{
	binary_tree_node_t *node1, *node2;
	symbol_t *symbol1, *symbol2;
	size_t freq1, freq2;

	node1 = (binary_tree_node_t *)p1;
	node2 = (binary_tree_node_t *)p2;

	symbol1 = (symbol_t *)node1;
	symbol2 = (symbol_t *)node2;

	freq1 = symbol1 ? symbol1->freq : 0;
	freq2 = symbol2 ? symbol2->freq : 0;

	if (freq1 > freq2)
		return (1);

	if (freq2 > freq1)
		return (-1);

	return (0);
}

/**
 * free_data - frees internal data of a node
 * @data: data (to be typecasted to a binary_tree_node_t node)
 **/
void free_data(void *data)
{
	binary_tree_node_t *node;

	if (data == NULL)
		return;

	node = (binary_tree_node_t *)data;
	if (node)
	{
		free(((binary_tree_node_t *)(node->data))->data);
		free(node->data);
	}
}
