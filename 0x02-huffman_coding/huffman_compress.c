#include "heap/heap.h"
#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define READ_SIZE 1024


/**
 * get_bit - returns a bit from a file
 * @fd: file descriptor
 * Return: a bit
 **/
int get_bit(int fd)
{
	static char chunk;
	static int bit_position;


	if (fd == -1)
	{
		bit_position = 0;
		return (0);
	}

	if (bit_position == 8 * sizeof(chunk) || bit_position == 0)
	{
		bit_position = 0;
		if (read(fd, &chunk, sizeof(chunk)) <= 0)
			return (-1);
	}


	return ((chunk >> ((8 * sizeof(chunk) - 1) - bit_position++)) & 1);
}

/**
 * encode_tree_delete_tree_return_codes - encodes a huffman tree to fd,
 *										  deletes tree, and returns codes
 * @tree: huffman tree
 * @fd: file descriptor where tree will be encoded
 * Return: huffman codes from huffman tree as an array
 **/
char **encode_tree_delete_tree_return_codes(binary_tree_node_t *tree, int fd)
{
	huffqueue_t queue = {NULL, NULL};
	huffman_list_node_t *tmp;
	binary_tree_node_t *node, *next_level = NULL;
	int i, depth = 0;
	char **codes, c;

	if (tree == NULL || huffqueue_add(tree, NULL, &queue) == 0)
		return (NULL);
	for (codes = calloc(ASCII_SIZE, sizeof(char *)); queue.head; queue.head = tmp)
	{
		node = queue.head->data, c = ((symbol_t *)node->data)->data;
		if (next_level == NULL || node == next_level)
		{
			next_level = node->left ? node->left : node->right;
			depth += !!next_level;
		}
		add_bit(!!node->left, fd);
		if (node->left)
		{
			queue.head->code[depth - 1] = '0';
			huffqueue_add(node->left, queue.head->code, &queue);
		}
		add_bit(!!node->right, fd);
		if (node->right)
		{
			queue.head->code[depth - 1] = '1';
			huffqueue_add(node->right, queue.head->code, &queue);
		}
		add_bit((c != -1), fd);
		if (c != -1)
		{
			codes[(int)c] = strdup(queue.head->code);
			for (i = 0; (size_t)i < 8 * sizeof(char); i++)
				add_bit(((c >> ((8 * sizeof(c) - 1) - i)) & 1), fd);
		}
		tmp = queue.head->next;
		free(node->data), free(node), free(queue.head);
	}
	add_bit(-1, fd);
	return (codes);
}

/**
 * compress - compresses data from in_fd and saves into out_fd
 * @in_fd: input file descriptor
 * @out_fd: output file descriptor
 **/
void compress(int in_fd, int out_fd)
{
	char *data, buffer[READ_SIZE], **codes;
	size_t freq_tmp[ASCII_SIZE], *freq;
	ssize_t i, j, bytes, symbol_count = 1;
	binary_tree_node_t *tree;

	/* READ FILE -- COUNT FREQUENCY OF SYMBOLS */
	memset(freq_tmp, 0, sizeof(freq_tmp));
	freq_tmp[0] = 1;
	memset(buffer, 0, sizeof(buffer));
	for (bytes = ARRAY_SIZE(buffer); bytes == ARRAY_SIZE(buffer); )
	{
		bytes = read(in_fd, buffer, ARRAY_SIZE(buffer));
		for (i = 0; i < bytes; i++)
		{
			symbol_count += (freq_tmp[(int)buffer[i]] == 0);
			freq_tmp[(int)buffer[i]] += 1;
		}
	}
	/* CREATE DATA/FREQ ARRAYS. USE TO CREATE HUFFMAN CODES */
	data = malloc(sizeof(char) * symbol_count);
	freq = malloc(sizeof(size_t) * symbol_count);
	for (i = 0, j = 0; (size_t)i < ARRAY_SIZE(freq_tmp); i++)
		if (freq_tmp[i] != 0)
			data[j] = (char)i, freq[j++] = freq_tmp[i];
	tree = huffman_tree(data, freq, symbol_count);
	codes = encode_tree_delete_tree_return_codes(tree, out_fd);
	/* WRITE TO FILE */
	lseek(in_fd, 0, SEEK_SET);
	for (bytes = ARRAY_SIZE(buffer); bytes == ARRAY_SIZE(buffer); )
	{
		bytes = read(in_fd, buffer, ARRAY_SIZE(buffer));
		for (i = 0; i < bytes; i++)
			huffman_write(codes[(int)buffer[i]], out_fd);
	}
	huffman_write(codes[0], out_fd);
	huffman_write(NULL, out_fd);
	/* FREE EVERYTHING */
	for (i = 0; codes && i < ASCII_SIZE; i++)
		free(codes[i]);
	free(codes), free(data), free(freq);
}

/**
 * get_char - traverses a huffman tree to find and return a character
 * @tree: huffman tree
 * @fd: file descriptor to pull traversal directions from
 * Return: found character
 **/
char get_char(binary_tree_node_t *tree, int fd)
{
	int i;

	while (tree->data == NULL)
	{
		i = get_bit(fd);
		if (i == 1)
			tree = tree->right;
		else
			tree = tree->left;
	}
	return (*((char *)tree->data));
}

/**
 * decompress - decompresses data from in_fd and saves into out_fd
 * @in_fd: input file descriptor
 * @out_fd: output file descriptor
 **/
void decompress(int in_fd, int out_fd)
{
	char datum[2] = {0, 0}, c;
	binary_tree_node_t *node = binary_tree_node(NULL, NULL), *root = NULL;
	huffqueue_t queue = {NULL, NULL};
	huffman_list_node_t *tmp;
	size_t i;

	/* DECODE HUFFMAN TREE */
	root = node;
	for (huffqueue_add(node, NULL, &queue); queue.head; queue.head = tmp)
	{
		node = queue.head->data;
		if (get_bit(in_fd) == 1)
		{
			node->left = binary_tree_node(node, NULL);
			huffqueue_add(node->left, NULL, &queue);
		}
		if (get_bit(in_fd) == 1)
		{
			node->right = binary_tree_node(node, NULL);
			huffqueue_add(node->right, NULL, &queue);
		}
		if (get_bit(in_fd) == 1)
		{
			for (i = 0; i < 8 * sizeof(*datum); i++)
				*datum |= get_bit(in_fd) << ((8 * sizeof(*datum) - 1) - i);
			node->data = strdup(datum);
			memset(datum, 0, sizeof(datum));
		}
		tmp = queue.head->next;
		free(queue.head);
	}


	/* DECOMPRESS FILE */
	get_bit(-1);
	while ((c = get_char(root, in_fd)))
		write(out_fd, &c, sizeof(c));

	tree_delete(root, free_data_simple);
}

/**
 * main - entry point for a huffman tree compression/decompression program
 * @argc: argument count
 * @argv: arguments array
 * Return: EXIT_SUCCESS on success | EXIT_FAILURE on failure
 **/
int main(int argc, char *argv[])
{
	int in_fd;
	int out_fd = 0;

	if (argc != 4)
	{
		printf("Usage: huffman <mode> <filename> <out_filename>\n");
		return (EXIT_FAILURE);
	}

	if (strcmp(argv[1], "c") && strcmp(argv[1], "d"))
	{
		printf("Mode must either be 'c' (compress) or 'd' (decompress)\n");
		return (EXIT_FAILURE);
	}

	in_fd = open(argv[2], O_RDONLY);
	if (in_fd == -1)
	{
		printf("No such file: %s\n", argv[2]);
		return (EXIT_FAILURE);
	}
	out_fd = open(argv[3], O_WRONLY | O_CREAT | O_EXCL, 0640);
	if (out_fd == -1)
	{
		printf("File already exists: %s\n", argv[3]);
		close(in_fd);
		return (EXIT_FAILURE);
	}

	if (argv[1][0] == 'c')
		compress(in_fd, out_fd);
	else
		decompress(in_fd, out_fd);

	close(in_fd);
	close(out_fd);
	return (EXIT_SUCCESS);
}
