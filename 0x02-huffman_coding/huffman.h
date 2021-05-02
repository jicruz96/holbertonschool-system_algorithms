#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "heap.h"

#ifndef NULL
#define NULL ((void *)0)
#endif /* NULL */

/**
 * struct symbol_s - Stores a char and its associated frequency
 *
 * @data: The character
 * @freq: The associated frequency
 */
typedef struct symbol_s
{
	char data;
	size_t freq;
} symbol_t;

symbol_t *symbol_create(char data, size_t freq);
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size);
int compare_frequencies(void *f1, void *f2);
void free_data(void *node);

int huffman_extract_and_insert(heap_t *priority_queue);
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size);


#endif /* HUFFMAN_H */
