#include "huffman.h"
#include <stdlib.h>

/**
 * symbol_create - creates a symbol_t data structure
 * @data: data to be stored in structure
 * @freq: freq to be stored in structure
 * Return: pointer to the created structure
 **/
symbol_t *symbol_create(char data, size_t freq)
{
	symbol_t *new = malloc(sizeof(symbol_t));

	if (new == NULL)
		return (NULL);

	new->data = data;
	new->freq = freq;
	return (new);
}
