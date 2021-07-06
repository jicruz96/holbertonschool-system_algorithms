#include "nary_trees.h"
#include <string.h>
#include <stdio.h>
/**
 * path_exists - checks if a path exists in a file directory represented by an
 *               N-ary tree
 *
 * @root: pointer to root of tree
 * @path: path to find in tree as an array of strings
 * Return: 1 if path in tree | 0 if not
 */
int path_exists(nary_tree_t const *root, char const * const *path)
{
	nary_tree_t *tmp;

	if (!root || !path || strcmp(root->content, *path))
		return (0);

	if (*(++path) == NULL)
		return (1);

	for (tmp = root->children; tmp; tmp = tmp->next)
		if (path_exists(tmp, path))
			return (1);

	return (0);
}
