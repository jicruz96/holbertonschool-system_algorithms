#include "rb_trees.h"
#include <stdlib.h>

/**
 * rb_tree_remove - removes a node from a red-black tree
 * @root: root of tree
 * @n: value to search and remove from tree
 * Return: pointer to root of tree
 **/
rb_tree_t *rb_tree_remove(rb_tree_t *root, int n)
{
    rb_tree_t *replacement, *node;
	
	node = rb_tree_find_node(root, n);
    if (node == NULL)
        return (root);

    /* Transfer node to a leaf position */
    while (node->right != NULL || node->left != NULL)
    {
        if (!node->right)
        {
            replacement = node->left;
        }
        else
        {
            replacement = node->right;
            while (replacement->left)
                replacement = replacement->left;
        }
        node->n = replacement->n;
        node = replacement;
    }

    if (node == root)
    {
        free(node);
        return (NULL);
    }

    if (node->color == BLACK)
		remove_repair(node, &root);

    if (node->parent->right == node)
		node->parent->right = NULL;
	else
		node->parent->left = NULL;
	free(node);
    return (root);
}

/**
 * remove_repair - repairs a red black tree that is about to have a black
 *                 leaf removed from it
 * @node: node
 * @root: double pointer to root of tree (in case we need to update it)
 **/
void remove_repair(rb_tree_t *node, rb_tree_t **root)
{
    rb_tree_t *sister = get_sister(node);
	
    if (is_red(sister))
	{
		node->parent->color = RED;
		sister->color = BLACK;
		if (node->parent == *root)
			*root = sister;
		if (node == node->parent->left)
			rotate_left(node->parent);
		else
			rotate_right(node->parent);
		remove_repair(node, root);		
	}
    else
	{
		if (!sister || (is_black(sister->left) && is_black(sister->right)))
		{
			if (sister)
				sister->color = RED;
			if (node != *root)
			{
				if (node->parent->color == RED)
					node->parent->color = BLACK;
				else
					remove_repair(node->parent, root);
			}
		}
		else
		{
        	rotate_parent(node, sister, root);
		}
	}
}

/**
 * rotate_parent - helper function for remove_repair. rotates parent towards
 *                 node. this function is called only when a node and its
 *                 sister are both black and the sisters children are
 *                 alternating colors
 * @node: node
 * @sister: node's sister
 * @root: double pointer to root of tree (in case we need to update it)
 **/
void rotate_parent(rb_tree_t *node, rb_tree_t *sister, rb_tree_t **root)
{
	rb_tree_t *close_nephew, *far_nephew;
	rb_color_t tmp_color;

	if (node == node->parent->left)
	{
		far_nephew = sister->right;
		close_nephew = sister->left;
	}
	else
	{
		far_nephew = sister->left;
		close_nephew = sister->right;
	}

	if (is_red(close_nephew))
	{
		sister->color = RED, close_nephew->color = BLACK;
		if (close_nephew == sister->left)
			rotate_right(sister);
		else
			rotate_left(sister);
		sister = get_sister(node);
		if (node == node->parent->left)
			far_nephew = sister->right, close_nephew = sister->left;
		else
			far_nephew = sister->left, close_nephew = sister->right;
	}

	far_nephew->color = BLACK;
	tmp_color = sister->color;
	sister->color = sister->parent->color;
	sister->parent->color = tmp_color;
	if (node->parent == *root)
		*root = sister;

	if (node == node->parent->left)
		rotate_left(node->parent);
	else
		rotate_right(node->parent);
}

/**
 * rb_tree_find_node - finds a node in a Red Black tree
 * @tree: pointer to tree
 * @n: value to find
 * Return: pointer to node with value n, or NULL if it doesn't exist
 **/
rb_tree_t *rb_tree_find_node(rb_tree_t *tree, int n)
{
    if (!tree)
        return (NULL);

    if (tree->n == n)
        return (tree);

    if (n > tree->n)
        return (rb_tree_find_node(tree->right, n));

    return (rb_tree_find_node(tree->left, n));
}

/**
 * rotate_left - executes binary tree left rotation
 * @node: node to rotate around
 * Return: 1 on success | 0 if unrotatable (either node or right child is null)
 **/
void rotate_left(rb_tree_t *node)
{
	rb_tree_t *parent = node->parent;
	rb_tree_t *rotator = node->right;

	if (!rotator)
		return;

	node->right = rotator->left;
	if (node->right)
		node->right->parent = node;

	rotator->left = node;
	node->parent = rotator;
	rotator->parent = parent;

	if (parent)
	{
		if (node == parent->right)
			parent->right = rotator;
		else
			parent->left = rotator;
	}
}

/**
 * rotate_right - executes binary tree right rotation
 * @node: node to rotate around
 * Return: 1 on success | 0 if unrotatable (either node or left child is null)
 **/
void rotate_right(rb_tree_t *node)
{
	rb_tree_t *parent = node->parent;
	rb_tree_t *rotator = node->left;

	if (!rotator)
		return;

	node->left = rotator->right;
	if (node->left)
		node->left->parent = node;

	rotator->right = node;
	node->parent = rotator;
	rotator->parent = parent;

	if (parent)
	{
		if (node == parent->right)
			parent->right = rotator;
		else
			parent->left = rotator;
	}
}
