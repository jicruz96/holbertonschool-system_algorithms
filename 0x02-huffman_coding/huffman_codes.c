#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * huffman_codes - makes huffman tree and prints huffman codes
 * @data: array of characters
 * @freq: array of associated frequencies
 * @size: sizes of arrays
 * Return: 1 on success | 0 on failure
 **/
int huffman_codes(char *data, size_t *freq, size_t size)
{
	return (print_huffman_codes(huffman_tree(data, freq, size)));
}

/**
 * print_huffman_codes - helper function for huffman_codes()
 * @ht: pointer to huffman tree
 * Return: 1 on success | 0 on failure
 **/
int print_huffman_codes(binary_tree_node_t *ht)
{
	huffqueue_t queue = {NULL, NULL};
	huffman_list_node_t *huffcode = queue.head;
	int depth = 0, count = 1, next_level = 2;
	binary_tree_node_t *node;
	symbol_t *symbol;

	if (ht == NULL || huffqueue_add(ht, NULL, &queue) == 0)
		return (0);

	for (huffcode = queue.head; huffcode; huffcode = queue.head)
	{
		if (count++ == next_level)
			depth += 1, next_level *= 2;

		node = huffcode->data;
		symbol = node->data;

		if (symbol->data != -1)
			printf("%c: %s\n", symbol->data, huffcode->code);

		if (node->left)
		{
			huffcode->code[depth] = '0';
			if (huffqueue_add(node->left, huffcode->code, &queue) == 0)
				return (0);
		}

		if (node->right)
		{
			huffcode->code[depth] = '1';
			if (huffqueue_add(node->right, huffcode->code, &queue) == 0)
				return (0);
		}

		queue.head = huffcode->next;
		free(symbol), free(node), free(huffcode);
	}

	return (1);
}


/**
 * huffqueue_add - adds node to a huffman_list_node_t queue
 * @node: new node to add to queue
 * @code: associated huffman code for the node
 * @queue: queue
 * Return: 1 on success | 0 on failure
 **/
int huffqueue_add(binary_tree_node_t *node, char *code, huffqueue_t *queue)
{
	huffman_list_node_t *huffcode = malloc(sizeof(huffman_list_node_t));

	if (huffcode == NULL)
		return (0);

	if (code)
	{
		memcpy(huffcode->code, code, sizeof(huffcode->code) / sizeof(*code));
	}
	else
	{
		memset(huffcode->code, '\0', sizeof(huffcode->code));
		huffcode->code[0] = '0';
	}

	huffcode->data = node;
	huffcode->next = NULL;

	if (queue->head == NULL)
	{
		queue->head = huffcode;
		queue->tail = huffcode;
	}
	else
	{
		queue->tail->next = huffcode;
		queue->tail = huffcode;
	}

	return (1);
}
