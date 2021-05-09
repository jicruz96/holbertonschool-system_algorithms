#include "heap/heap.h"
#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * free_data_simple - frees data inside a huffman tree node
 * @node: pointer to a huffman tree node
 **/
void free_data_simple(void *node)
{
	free(((binary_tree_node_t *)node)->data);
}

/**
 * huffman_write - writes a huffman code into a file
 * @code: huffman code
 * @fd: file to write to
 **/
void huffman_write(char *code, int fd)
{
	static char chunk;
	static int bit_position;

	if (!code)
	{
		write(fd, &chunk, sizeof(chunk));
		bit_position = 0;
		return;
	}

	for (; *code; code++, bit_position++)
	{
		if (bit_position == 8 * sizeof(chunk))
		{
			write(fd, &chunk, sizeof(chunk));
			memset(&chunk, 0, sizeof(chunk));
			bit_position = 0;
		}

		if (*code == '1')
			chunk |= 1 << ((8 * sizeof(chunk) - 1) - bit_position);
	}
}

/**
 * add_bit - adds a bit to 1-byte chunk. Writes to file when chunk is full
 * @bit: 1 or 0
 * @fd: output file descriptor
 **/
void add_bit(int bit, int fd)
{
	static char chunk;
	static int bit_position;

	if (bit_position == 8 * sizeof(chunk) || bit == -1)
	{
		write(fd, &chunk, sizeof(chunk));
		memset(&chunk, 0, sizeof(chunk));
		bit_position = 0;
		if (bit == -1)
			return;
	}
	if (bit == 1)
		chunk |= 1 << ((8 * sizeof(chunk) - 1) - bit_position);
	bit_position += 1;
}
