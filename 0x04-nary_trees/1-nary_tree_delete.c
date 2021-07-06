#include "nary_trees.h"

/**
 * nary_tree_delete - deallocates a N-ary tree
 * @tree: pointer to tree
 */
void nary_tree_delete(nary_tree_t *tree)
{
	nary_tree_t *tmp, *tmp2;

	if (!tree)
		return;

	for (tmp = tree->children; tmp; tmp = tmp2)
		tmp2 = tmp->next, nary_tree_delete(tmp);

	free(tree->content);
	free(tree);
}
