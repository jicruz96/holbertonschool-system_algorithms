#include "heap.h"
#include <stdlib.h>

/**
 * heap_delete - deallocates a heap_t data structure
 * @heap: pointer to heap
 * @free_data: pointer to a function used to free the content of a node
 **/
void heap_delete(heap_t *heap, void (*free_data)(void *))
{
	if (heap == NULL)
		return;

	tree_delete(heap->root, free_data);
	free(heap);
}

/**
 * tree_delete - deletes a binary tree, using free_data to delete node contents
 * @tree: pointer to tree
 * @free_data: pointer to function used to free node contents
 **/
void tree_delete(binary_tree_node_t *tree, void (*free_data)(void *))
{
	if (tree == NULL)
		return;

	tree_delete(tree->left, free_data);
	tree_delete(tree->right, free_data);
	if (free_data)
		free_data((void *)tree);
	free(tree);
}
