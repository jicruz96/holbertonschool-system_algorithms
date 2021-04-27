#include "heap.h"
#include <stdlib.h>

/**
 * heap_create - allocates memory for a new heap_t object
 * @data_cmp: data comparison algorithm to be used by heap_t object
 * Return: pointer to heap object
 **/
heap_t *heap_create(int (*data_cmp)(void *, void *))
{
	heap_t *new = malloc(sizeof(heap_t));

	if (new == NULL)
		return (NULL);

	new->data_cmp = data_cmp;
	new->root = NULL;
	new->size = 0;
	return (new);
}
