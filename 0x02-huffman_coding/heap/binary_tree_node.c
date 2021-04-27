#include "heap.h"
#include <stdlib.h>

/**
 * binary_tree_node - allocates memory for a new binary_tree_node_t object
 * @parent: parent of new node
 * @data: data of new node
 * Return: pointer to new node
 **/
binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data)
{
	binary_tree_node_t *new = malloc(sizeof(binary_tree_node_t));

	if (new == NULL)
		return (NULL);

	new->parent = parent;
	new->left = NULL;
	new->right = NULL;
	new->data = data;
	return (new);
}
