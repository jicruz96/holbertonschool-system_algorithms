#include "nary_trees.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

static size_t nary_tree_depth(nary_tree_t const *root);

/**
 * nary_tree_diameter - computes and returns the diameter of an N-ary tree
 *
 * @root: pointer to root of tree
 * Return: tree diameter
 */
size_t nary_tree_diameter(nary_tree_t const *root)
{
	size_t max_depth = 0, second_max_depth = 0, max_diam = 0;
	size_t tmp_depth, tmp_diam, tmp;
	nary_tree_t *tmp_node;

	if (!root)
		return (0);

	for (tmp_node = root->children; tmp_node; tmp_node = tmp_node->next)
	{
		tmp_depth = nary_tree_depth(tmp_node);
		tmp_diam  = nary_tree_diameter(tmp_node);
		max_diam = max(tmp_diam, max_diam);
		if (tmp_depth > max_depth)
		{
			tmp = max_depth;
			max_depth = tmp_depth;
			second_max_depth = tmp;
		}
		else if (tmp_depth > second_max_depth)
		{
			second_max_depth = tmp_depth;
		}
	}

	return (max(max_diam, max_depth + second_max_depth + 1));
}

/**
 * nary_tree_depth - computes the depth of an nary tree
 *
 * @root: pointer to tree
 * Return: depth of tree
 */
static size_t nary_tree_depth(nary_tree_t const *root)
{
	size_t max_depth = 0, tmp_depth;
	nary_tree_t *tmp;

	if (!root)
		return (0);

	for (tmp = root->children; tmp; tmp = tmp->next)
	{
		tmp_depth = nary_tree_depth(tmp);
		max_depth = max(tmp_depth, max_depth);
	}

	return (max_depth + 1);
}
