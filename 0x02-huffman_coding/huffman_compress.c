#include "heap/heap.h"
#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define READ_SIZE 1
#define ASCII_SIZE (1 << 8)


/**
 * compress - compresses data from in_fd and saves into out_fd
 * @in_fd: input file descriptor
 * @out_fd: output file descriptor
 **/
void compress(int in_fd, int out_fd)
{
	char *data, buffer[READ_SIZE];
	size_t freq[ASCII_SIZE], *f2;
	ssize_t i, j, bytes, symbol_count = 0;

	memset(freq, 0, sizeof(freq) / sizeof(freq[0]));
	bytes = read(in_fd, buffer, sizeof(buffer) / sizeof(buffer[0]));
	while (bytes > 0)
	{
		for (i = 0; i < bytes; i++)
		{
			if (freq[(int)buffer[i]] == 0)
				symbol_count += 1;
			freq[(int)buffer[i]] += 1;
		}
		bytes = read(in_fd, buffer, sizeof(buffer) / sizeof(buffer[0]));
		printf("bytes read: %zd\n", bytes);
	}
	data = malloc(sizeof(char) * symbol_count);
	f2 = malloc(sizeof(size_t) * symbol_count);
	for (i = 0, j = 0; (size_t)i < sizeof(freq) / sizeof(freq[0]); i++)
	{
		if (freq[i] != 0)
		{
			data[j] = (char)i;
			f2[j] = freq[i];
			j++;
		}
	}
	
	huffman_codes(data, f2, symbol_count);
	(void)out_fd;
	/* make the frequency table */
	/* make the huffman tree */
}

/**
 * decompress - decompresses data from in_fd and saves into out_fd
 * @in_fd: input file descriptor
 * @out_fd: output file descriptor
 **/
void decompress(int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
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

	/*out_fd = open(argv[3], O_WRONLY | O_CREAT | O_EXCL);
	if (out_fd == -1)
	{
		printf("File already exists: %s\n", argv[3]);
		close(in_fd);
		return (EXIT_FAILURE);
	}*/

	if (argv[1][0] == 'c')
		compress(in_fd, out_fd);
	else
		decompress(in_fd, out_fd);

	close(in_fd);
	close(out_fd);
	return (EXIT_SUCCESS);
}
