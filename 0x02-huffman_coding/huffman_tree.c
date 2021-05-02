#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

/**
 * huffman_tree - builds a huffman tree
 * @data: array of characters of size `size`
 * @freq: array of associated frequencies
 * @size: size of arrays
 * Return: pointer to the root node of the huffman tree
 **/
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size)
{
	heap_t *pq = huffman_priority_queue(data, freq, size);
	binary_tree_node_t *huffman_tree;

	if (pq == NULL || pq->root == NULL)
		return (NULL);

	while (pq->size > 1)
		if (huffman_extract_and_insert(pq) == 0)
			return (NULL);

	huffman_tree = pq->root->data;
	heap_delete(pq, NULL);
	return (huffman_tree);
}
