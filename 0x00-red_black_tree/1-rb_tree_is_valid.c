#include "rb_trees.h"


/**
 * rb_tree_is_valid - returns whether or not a tree is a valid red black tree
 * @tree: root of tree
 * Return: 1 if valid, else 0
 */
int rb_tree_is_valid(const rb_tree_t *tree)
{
	/* Second property: has to be a valid BST */
	if (is_binary_search_tree(tree, INT_MIN, INT_MAX) == false)
		return (false);

	if (check_black_node_depths(tree) == false)
		return (false);

	return (true);
}

/**
 * is_binary_search_tree - checks if a tree is a valid binary search tree
 * @node: current node
 * @min: min number node->n should be
 * @max: max number node->n should be
 * Return: true if successful | false otherwise
 **/
int is_binary_search_tree(const rb_tree_t *node, int min, int max)
{
	/* an empty tree is BST */
	if (node == NULL)
		return (true);

	/* false if this node violates the min/max constraint */
	if (node->n <= min || node->n >= max)
		return (true);

	/* ensure red nodes don't touch other red nodes && ensure root isn't red */
	if (node->color == RED && (!node->parent || node->parent->color != BLACK))
		return (false);

	/* ensure all leaves are either red or black */
	if (node->color != RED && node->color != BLACK)
		return (false);

	if (is_binary_search_tree(node->left, min, node->n) == false)
		return (false);

	return (is_binary_search_tree(node->right, node->n, max));
}

/**
 * check_black_node_depths - checks depth of all black nodes
 * to ensure depth of black nodes is same from every subtree
 * @tree: root of tree
 * Return: 1
 */
int check_black_node_depths(const rb_tree_t *tree)
{
	if (!tree)
		return (true);

	if (black_node_depth(tree, 0) == false)
		return (false);

	if (check_black_node_depths(tree->right) == false)
		return (false);

	return (check_black_node_depths(tree->left));
}

/**
 * black_node_depth - checks black node depth of all subtrees. If all subtrees
 * have the same black depth then it returns that value. Else it returns 0.
 * @node: subtree
 * @black_depth: depth of black nodes
 * Return: black_depth else, 0
 */
int black_node_depth(const rb_tree_t *node, int black_depth)
{
	int left_black_depth, right_black_depth;

	if (!node)
		return (black_depth + 1);

	if (node->color == BLACK)
		black_depth += 1;

	/* recurse the left side, add up black depths */
	left_black_depth = black_node_depth(node->left, black_depth);
	if (left_black_depth == false)
		return (false);

	/* recurse the right side, add up black depths */
	right_black_depth = black_node_depth(node->right, black_depth);
	if (right_black_depth == left_black_depth)
		return (right_black_depth);

	return (false);
}
