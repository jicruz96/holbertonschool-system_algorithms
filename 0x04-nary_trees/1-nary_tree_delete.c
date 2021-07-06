#include "nary_trees.h"

/**
 * nary_tree_delete - deallocates a N-ary tree
 * @tree: pointer to tree
 */
void nary_tree_delete(nary_tree_t *tree)
{
	if (!tree)
		return;

	for (; tree->children; tree->children = tree->children->next)
		nary_tree_delete(tree->children);

	free(tree->content);
	free(tree);
}
