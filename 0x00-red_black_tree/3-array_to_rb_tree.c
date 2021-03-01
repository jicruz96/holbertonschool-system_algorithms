#include "rb_trees.h"

/**
 * array_to_rb_tree - turns an array into a binary search tree
 * @array: array of integers
 * @size: size of array
 * Return: pointer to root node of RB-tree or NULL if failure occurs
 **/
rb_tree_t *array_to_rb_tree(int *array, size_t size)
{
	unsigned int i;
	rb_tree_t *tree = NULL;

	for (i = 0; i < size; i++)
		rb_tree_insert(&tree, array[i]);

	return (tree);
}
