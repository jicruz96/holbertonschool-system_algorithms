#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * get_huffman_codes - returns the huffman codes for an array of data
 * @data: array of characters
 * @freq: array of associated frequencies
 * @size: sizes of arrays
 * Return: pointer to array of huffman codes or NULL on failure
 **/
char **get_huffman_codes(char *data, size_t *freq, size_t size, int out_fd)
{
	binary_tree_node_t *ht = huffman_tree(data, freq, size);
	huffqueue_t queue = {NULL, NULL};
	huffman_list_node_t *huffcode = queue.head;
	binary_tree_node_t *node, *next_level = NULL;
	symbol_t *symbol;
	int depth = 0;
    char **codes;

	if (ht == NULL || huffqueue_add(ht, NULL, &queue) == 0)
		return (NULL);
    huffman_codes(data, freq, size);
    codes = calloc(ASCII_SIZE, sizeof(char *));

	for (huffcode = queue.head; huffcode; huffcode = queue.head)
	{
		node = huffcode->data;
		if (next_level == NULL || node == next_level)
		{
			next_level = node->left ? node->left : node->right;
			depth += (next_level != NULL);
		}

        write(out_fd, node->left ? "1" : "0", 1);
        write(out_fd, node->right ? "1" : "0", 1);
		symbol = node->data;
        write(out_fd, symbol->data != -1 ? "1" : "0", 1);
		if (symbol->data != -1)
        {
            codes[(int)symbol->data] = strdup(huffcode->code);
            write(out_fd, &symbol->data, 1);
        } 


		if (node->left)
		{
			huffcode->code[depth - 1] = '0';
			if (huffqueue_add(node->left, huffcode->code, &queue) == 0)
				return (0);
		}
		if (node->right)
		{
			huffcode->code[depth - 1] = '1';
			if (huffqueue_add(node->right, huffcode->code, &queue) == 0)
				return (0);
		}

		queue.head = huffcode->next;
		free(symbol), free(node), free(huffcode);
	}

	return (codes);
}

