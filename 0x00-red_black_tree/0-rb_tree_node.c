#include "rb_trees.h"
#include <stdlib.h>

/**
 * rb_tree_node - creates a Red-Black Tree node
 * @parent: pointer to the parent node of the node to create
 * @value: value to put in the new node
 * @color: node color (either RED (0) or BLACK (1))
 * Return: pointer to new node or NULL on failure
 **/
rb_tree_t *rb_tree_node(rb_tree_t *parent, int value, rb_color_t color)
{
	rb_tree_t *node = malloc(sizeof(rb_tree_t));

	if (!node)
		return (NULL);

	node->color  = color;
	node->n      = value;
	node->left   = NULL;
	node->right  = NULL;
	node->parent = parent;

	if (parent)
	{
		if (parent->n < value)
			parent->right = node;
		else
			parent->left = node;
	}

	return (node);
}
