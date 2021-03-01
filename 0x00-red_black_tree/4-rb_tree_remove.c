#include "rb_trees.h"
#include "2-rb_tree_insert.c"
#include <stdlib.h>

void red_sister_cases(rb_tree_t *node, rb_tree_t *sister, rb_tree_t **root);
void black_sister_cases(rb_tree_t *node, rb_tree_t *sister, rb_tree_t **root);
void sister_cases(rb_tree_t *node, rb_tree_t **root);
rb_tree_t *find_node(rb_tree_t *node, int n);

/**
 * rb_tree_remove - removes a node from a red-black tree
 * @root: root of tree
 * @n: value to search and remove from tree
 * Return: pointer to root of tree
 **/
rb_tree_t *rb_tree_remove(rb_tree_t *root, int n)
{
    rb_tree_t *node = find_node(root, n);
    rb_tree_t *replacement;
    rb_tree_t *tmp;

    /* Case 0: Value does not exist in the tree */
    if (!node)
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

    /* Case 1: Node is root */
    if (node == root)
    {
        free(node);
        return (NULL);
    }

    /* Case 2: node is red leaf */
    if (node->color == RED)
    {
        if (node->parent->right == node)
            node->parent->right = NULL;
        else
            node->parent->left = NULL;
        free(node);
        return (root);
    }
    
    /* Case 3: node is black leaf */
    tmp = node;
    sister_cases(node, &root);
    if (tmp->parent->right == tmp)
		tmp->parent->right = NULL;
	else
		tmp->parent->left = NULL;
	free(tmp);

    return (root);
    
}

void sister_cases(rb_tree_t *node, rb_tree_t **root)
{
    rb_tree_t *sister;
	
	sister = get_sister(node);
    if (is_red(sister))
        red_sister_cases(node, sister, root);
    else
        black_sister_cases(node, sister, root);
}

void black_sister_cases(rb_tree_t *node, rb_tree_t *sister, rb_tree_t **root)
{
	rb_tree_t *close_nephew, *far_nephew;
	rb_color_t tmp_color;

    if (!sister || (is_black(sister->left) && is_black(sister->right)))
    {
        if (sister)
            sister->color = RED;
        
        if (node != *root)
        {
            if (node->parent->color == BLACK)
			{
				sister = get_sister(node->parent);
                sister_cases(node->parent, root);
			}
            else
			{
                node->parent->color = BLACK;
			}
        }
        return;
    }

	if (node == node->parent->left)
		far_nephew = sister->right, close_nephew = sister->left;
	else
		far_nephew = sister->left, close_nephew = sister->right;

	if (is_red(close_nephew))
	{
		sister->color = RED;
		close_nephew->color = BLACK;
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

	tmp_color = sister->color;
	sister->color = sister->parent->color;
	sister->parent->color = tmp_color;
	if (node->parent == *root)
		*root = sister;
	if (node == node->parent->left)
		rotate_left(node->parent);
	else
		rotate_right(node->parent);
	far_nephew->color = BLACK;
	
}

void red_sister_cases(rb_tree_t *node, rb_tree_t *sister, rb_tree_t **root)
{
	node->parent->color = RED;
	sister->color = BLACK;
	if (node->parent == *root)
		*root = sister;
	if (node == node->parent->left)
		rotate_left(node->parent);
	else
		rotate_right(node->parent);
	sister_cases(node, root);
}

void swap_nodes(rb_tree_t *n1, rb_tree_t *n2)
{
    rb_tree_t *n1_parent, *n1_left, *n1_right;
    rb_tree_t *n2_parent, *n2_left, *n2_right;

    if (!n1 || !n2)
        return;
    /* assign */    
    n1_parent = n1->parent, n1_left = n1->left, n1_right = n1->right;
    n2_parent = n2->parent, n2_left = n2->left, n2_right = n2->right;
    /* swap */
    n1->parent = n2_parent, n1->left = n2_left, n1->right = n2_right;
    n2->parent = n1_parent, n2->left = n1_left, n2->right = n1_right;
    
    if (n1_parent)
    {
        if (n1 == n1_parent->right)
            n1->parent->right = n2;
        else
            n1->parent->left = n2;
    }

    if (n2_parent)
    {
        if (n2 == n2_parent->right)
            n2->parent->right = n1;
        else
            n2->parent->left = n1;
    }

    if (n1_right)
        n1->right->parent = n2;
    if (n1_left)
        n1->left->parent = n2;
    if (n2_right)
        n2->right->parent = n1;
    if (n2_left)
        n2->left->parent = n1;
}


rb_tree_t *find_node(rb_tree_t *node, int n)
{
    if (!node)
        return (NULL);

    if (node->n == n)
        return (node);

    if (n > node->n)
        return (find_node(node->right, n));

    return (find_node(node->left, n));
}
