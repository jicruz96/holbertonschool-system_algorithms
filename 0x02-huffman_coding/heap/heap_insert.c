#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * heap_insert - inserts a new value into a min binary heap
 * @heap: pointer to min heap
 * @data: pointer to new data to add to heap
 * Return: pointer to new node
 **/
binary_tree_node_t *heap_insert(heap_t *heap, void *data)
{
	binary_tree_node_t *tmp_node, *new = binary_tree_node(NULL, data);
	size_t tmp, moves;

	if (new == NULL)
		return (NULL);

	heap->size += 1;
	if (heap->root)
	{
		/* moves = floor(log2(head->size)) - 1; but i can't use math.h >:( */
		for (tmp = 4, moves = 0; heap->size >= tmp; tmp *= 2)
			moves += 1;

		for (tmp_node = heap->root; moves; moves--)
			if (heap->size & (1 << moves))
				tmp_node = tmp_node->right;
			else
				tmp_node = tmp_node->left;

		if (heap->size & 1)
			tmp_node->right = new;
		else
			tmp_node->left = new;

		new->parent = tmp_node;

		while (new->parent)
			if (heap->data_cmp(new->data, new->parent->data) < 0)
				swap_node_with_parent(new);
			else
				break;
	}

	if (new->parent == NULL)
		heap->root = new;

	return (new);
}

/**
 * swap_node_with_parent - swaps position of a node with its parent in a tree
 * @node: binary tree node
 **/
void swap_node_with_parent(binary_tree_node_t *node)
{
	binary_tree_node_t *tmp, *parent;

	if (node == NULL || node->parent == NULL)
		return;

	parent = node->parent;

	if (parent->parent && parent->parent->right == parent)
		parent->parent->right = node;
	else if (parent->parent)
		parent->parent->left = node;

	node->parent = parent->parent;
	parent->parent = node;

	if (node->left)
		node->left->parent = parent;
	if (node->right)
		node->right->parent = parent;

	if (parent->right == node)
	{
		if (parent->left)
			parent->left->parent = node;
		parent->right = node->right;
		node->right = parent;
		tmp = parent->left;
		parent->left = node->left;
		node->left = tmp;
	}
	else
	{
		if (parent->right)
			parent->right->parent = node;
		parent->left = node->left;
		node->left = parent;
		tmp = parent->right;
		parent->right = node->right;
		node->right = tmp;
	}
}
