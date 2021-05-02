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

	if (pq == NULL)
		return (NULL);

	while (huffman_extract_and_insert(pq))
		continue;

	huffman_tree = pq->root;
	pq->root = NULL;
	free(pq);

	return (huffman_tree);
}
