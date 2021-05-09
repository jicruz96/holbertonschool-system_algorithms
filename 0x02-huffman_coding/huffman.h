#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "heap.h"

#ifndef NULL
#define NULL ((void *)0)
#endif /* NULL */

#define ASCII_SIZE (1 << 8)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

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

/**
 * struct huffman_list_node_s - linked list node that associates a huffman code
 * with a symbol struct stored in a binary tree node
 * @code: huffman code as a string
 * @data: pointer to binary tree node containing symbol associated with code
 * @next: next huffman code struct in the list
 **/
typedef struct huffman_list_node_s
{
	char code[256];
	binary_tree_node_t *data;
	struct huffman_list_node_s *next;
} huffman_list_node_t;

/**
 * struct huffqueue_s - queue for huffman code structs
 * @head: head of list
 * @tail: tail of list
 **/
typedef struct huffqueue_s
{
	huffman_list_node_t *head;
	huffman_list_node_t *tail;
} huffqueue_t;

symbol_t *symbol_create(char data, size_t freq);
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size);
int compare_frequencies(void *f1, void *f2);
void free_data(void *node);

int huffman_extract_and_insert(heap_t *priority_queue);
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size);

/* huffman_codes.c */
int huffman_codes(char *data, size_t *freq, size_t size);
int print_huffman_codes(binary_tree_node_t *ht);
int huffqueue_add(binary_tree_node_t *node, char *code, huffqueue_t *queue);

int symbol_print(char *buffer, void *data);

/* get_huffman_codes.c */
char **encode_tree_delete_tree_return_codes(binary_tree_node_t *tree, int fd);

/* make pretty later */
void free_data_simple(void *node);
void huffman_write(char *code, int fd);
void add_bit(int bit, int fd);
int get_bit(int fd);
char **encode_tree_delete_tree_return_codes(binary_tree_node_t *tree, int fd);
void compress(int in_fd, int out_fd);
char get_char(binary_tree_node_t *tree, int fd);
void decompress(int in_fd, int out_fd);








#endif /* HUFFMAN_H */
