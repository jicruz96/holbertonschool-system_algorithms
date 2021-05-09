#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * heap_extract - extacts the root value from a min binary heap
 * @heap: pointer to min heap
 * Return: pointer to data
 **/
void *heap_extract(heap_t *heap)
{
	void *data;
	binary_tree_node_t *node;
	int tmp, moves;

	if (heap == NULL || heap->root == NULL)
		return (NULL);

	data = heap->root->data;

	if (heap->size == 1)
	{
		free(heap->root);
		heap->root = NULL;
	}
	else
	{
		/* moves = floor(log2(head->size)) - 1; but i can't use math.h >:( */
		for (tmp = 4, moves = 0; (int)heap->size >= tmp; tmp *= 2)
			moves += 1;

		for (node = heap->root; moves >= 0; moves--)
			if (heap->size & (1 << moves))
				node = node->right;
			else
				node = node->left;

		heap->root->data = node->data;
		if (node->parent->right == node)
			node->parent->right = NULL;
		else
			node->parent->left = NULL;
		heapify(heap->root, heap);
		free(node);
	}
	heap->size -= 1;
	return (data);
}

/**
 * heapify - heapifies a broken heap
 * @node: current node of interest
 * @heap: heap
 **/
void heapify(binary_tree_node_t *node, heap_t *heap)
{
	binary_tree_node_t *smallest;

	if (node == NULL)
		return;

	if (node->left && heap->data_cmp(node->left->data, node->data) <= 0)
		smallest = node->left;
	else
		smallest = node;

	if (node->right)
	{
		if (smallest == node->left)
		{
			if (heap->data_cmp(node->right->data, smallest->data) < 0)
				smallest = node->right;
		}
		else if (heap->data_cmp(node->right->data, smallest->data) <= 0)
		{
			smallest = node->right;
		}
	}

	if (smallest != node)
	{
		swap_node_with_parent(smallest);
		if (smallest->parent == NULL)
			heap->root = smallest;
		heapify(node, heap);
	}
}
