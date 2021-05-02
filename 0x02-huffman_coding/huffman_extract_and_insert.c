#include "huffman.h"

/**
 * huffman_extract_and_insert - extracts two nodes of a priority queue and
 * inserts a new one
 * @priority_queue: pointer to the priority queue object
 * Return: 1 on success | 0 on failure
 **/
int huffman_extract_and_insert(heap_t *priority_queue)
{
	binary_tree_node_t *node1, *node2, *node;
	size_t freq1, freq2;
	symbol_t *symbol;

	if (priority_queue == NULL || priority_queue->size < 2)
		return (0);

	node1       = heap_extract(priority_queue);
	node2       = heap_extract(priority_queue);
	if (node1 == NULL || node2 == NULL)
		return (0);
	freq1       = ((symbol_t *)node1->data)->freq;
	freq2       = ((symbol_t *)node2->data)->freq;

	symbol = symbol_create(-1, freq1 + freq2);
	if (symbol == NULL)
		return (0);

	node        = binary_tree_node(NULL, symbol);
	if (node == NULL)
		return (0);
	node->left  = node1, node1->parent = node;
	node->right = node2, node2->parent = node;

	if (heap_insert(priority_queue, node) == NULL)
		return (0);

	return (1);
}
