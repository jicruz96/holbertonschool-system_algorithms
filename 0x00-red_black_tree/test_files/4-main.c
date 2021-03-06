#include <stdlib.h>
#include "rb_trees.h"

void rb_tree_print(const rb_tree_t *tree);

/**
 * main - Entry point
 *
 * Return: 0 on success, error code on failure
 */
int main(void)
{
    rb_tree_t *tree;
    int array[] = {
        79, 47, 68, 87, 84, 91, 21, 32, 34, 2,
        20, 22
    };
    size_t n = sizeof(array) / sizeof(array[0]);
    unsigned int i;

    tree = array_to_rb_tree(array, n);
    if (!tree)
        return (1);
    if (rb_tree_is_valid(tree))
        rb_tree_print(tree);
    else
        printf("The Price Is Right Tuba.\n");

    for (i = 0; i < sizeof(array) / sizeof(int); i++)
    {
        printf("\n---------\n\nREMOVING %d:\n\n", array[i]);
        tree = rb_tree_remove(tree, array[i]);
        if (rb_tree_is_valid(tree))
            rb_tree_print(tree);
        else
            printf("The Price Is Right Tuba.\n");
    }
    return (0);
}

/**
 * rb_tree_insert - inserts a value into a red-black tree
 * @tree: pointer to root of tree
 * @value: value to insert
 * Return: pointer to new node containing value
 **/
rb_tree_t *rb_tree_insert(rb_tree_t **tree, int value)
{
	rb_tree_t *node = bst_insert(tree, value);

	if (!node)
		return (NULL);

	insert_repair(tree, node);

	return (node);
}

/**
 * insert_repair - repairs RB-tree after a new node has been inserted
 * @tree: double pointer to root of tree
 * @node: current node to repair around
 **/
void insert_repair(rb_tree_t **tree, rb_tree_t *node)
{
	rb_tree_t *aunt = NULL;

	if (!node->parent)
	{
		node->color = BLACK;
		*tree = node;
	}
	else if (node->parent->color == RED)
	{
		if (node->parent->parent)
		{
			if (node->parent == node->parent->parent->right)
				aunt = node->parent->parent->left;
			else
				aunt = node->parent->parent->right;
		}
	
		if (aunt && aunt->color == RED)
			repair_case1(tree, node->parent);
		else
			repair_case2(tree, node);
	}
}

/**
 * repair_case1 - recolors parent, aunt, and grandparent; restarts repair
 *                process starting at grandparent position.
 * @tree: double pointer to root of tree
 * @parent: parent of inserted node
 **/
void repair_case1(rb_tree_t **tree, rb_tree_t *parent)
{
	rb_tree_t *grandparent = parent->parent;
	rb_tree_t *aunt = NULL;

	parent->color = BLACK;
	grandparent->color = RED;
	if (parent == grandparent->right)
		aunt = grandparent->left;
	else
		aunt = grandparent->right;
	if (aunt)
		aunt->color = BLACK;
	insert_repair(tree, grandparent);
}

/**
 * repair_case2 - performs rotations that get new node onto position of its
 *                grandparent
 * @tree: double pointer to root of tree
 * @node: current node to repair around
 **/
void repair_case2(rb_tree_t **tree, rb_tree_t *node)
{
	rb_tree_t *parent = node->parent;
	rb_tree_t *grandparent = parent->parent;

	if (node == parent->right && parent == grandparent->left)
	{
		rotate_left(parent);
		node = node->left;
	}
	else if (node == parent->left && parent == grandparent->right)
	{
		rotate_right(parent);
		node = node->right;
	}

	parent = node->parent;
	grandparent = parent->parent;

	if (node == parent->left)
		rotate_right(grandparent);
	else
		rotate_left(grandparent);

	if (parent->parent == NULL)
		*tree = parent;
	parent->color = BLACK;
	grandparent->color = RED;
}

/**
 * bst_insert - insert a value in a binary search tree
 * @tree: pointer to tree
 * @value: value to insert into tree
 * Return: pointer to created node
 **/
rb_tree_t *bst_insert(rb_tree_t **tree, int value)
{
	rb_tree_t *tmp;

	if (!(*tree))
	{
		*tree = rb_tree_node(NULL, value, RED);
		return (*tree);
	}

	tmp = *tree;

	if (value == tmp->n)
		return (NULL);

	if (value > tmp->n)
	{
		if (tmp->right)
			return (bst_insert(&(tmp->right), value));

		tmp->right = rb_tree_node(tmp, value, RED);
		return (tmp->right);
	}

	if (tmp->left)
		return (bst_insert(&(tmp->left), value));

	tmp->left = rb_tree_node(tmp, value, RED);
	return (tmp->left);
}
