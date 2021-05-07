#include "heap/heap.h"
#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define READ_SIZE 1024

void bah(void *tree)
{
    free(((binary_tree_node_t *)tree)->data);
}

/**
 * compress - compresses data from in_fd and saves into out_fd
 * @in_fd: input file descriptor
 * @out_fd: output file descriptor
 **/
void compress(int in_fd, int out_fd)
{
	char *data, buffer[READ_SIZE], **codes, *code, chunk;
	size_t freq[ASCII_SIZE], *f2;
	ssize_t i, j, bytes, symbol_count = 1;
    
    memset(freq, 0, sizeof(freq));
	bytes = read(in_fd, buffer, ARRAY_SIZE(buffer));
	freq[0] = 1;
    while (bytes > 0)
	{
		for (i = 0; i < bytes; i++)
		{
			if (freq[(int)buffer[i]] == 0)
				symbol_count += 1;
			freq[(int)buffer[i]] += 1;
		}
		bytes = read(in_fd, buffer, ARRAY_SIZE(buffer));
	}
	data = malloc(sizeof(char) * symbol_count);
	f2 = malloc(sizeof(size_t) * symbol_count);
	for (i = 0, j = 0; (size_t)i < ARRAY_SIZE(freq); i++)
	{
		if (freq[i] != 0)
		{
			data[j] = (char)i;
			f2[j] = freq[i];
			j++;
		}
	}

	codes = get_huffman_codes(data, f2, symbol_count, out_fd);
    lseek(in_fd, 0, SEEK_SET);
    bytes = read(in_fd, buffer, ARRAY_SIZE(buffer));
    memset(&chunk, 0, sizeof(chunk));
    j = 0;
    while (bytes > 0)
    {
        for (i = 0; i < bytes; i++)
        {
            for(code = codes[(int)buffer[i]]; *code; code++)
            {
                if (*code == '1')
                    chunk = chunk | (1 << (7 - j));
                j += 1;
                if (j == sizeof(chunk))
                {
                    write(out_fd, &chunk, sizeof(chunk));
                    j = 0;
                    memset(&chunk, 0, sizeof(chunk));
                }
            }
        }
        bytes = read(in_fd, buffer, ARRAY_SIZE(buffer));
    }

    for (code = codes[0]; *code; code++)
    {
        if (*code == '1')
            chunk = chunk | (1 << (7 - j));
        j += 1;
        if (j == sizeof(chunk))
        {
            write(out_fd, &chunk, sizeof(chunk));
            j = 0;
            memset(&chunk, 0, sizeof(chunk));
        }
    }

    if (j != 0)
        write(out_fd, &chunk, sizeof(chunk));

    if (codes)
    {
        for (i = 0; i < ASCII_SIZE; i++)
            free(codes[i]);

        free(codes);
    }
    free(data);
    free(f2);
}

/**
 * decompress - decompresses data from in_fd and saves into out_fd
 * @in_fd: input file descriptor
 * @out_fd: output file descriptor
 **/
void decompress(int in_fd, int out_fd)
{
    char datum[2], tree_buffer[3], buffer;
    binary_tree_node_t *root, *node;
    huffqueue_t queue = {NULL, NULL};
    ssize_t i, bytes;
    huffman_list_node_t *prev;
    
    
    datum[1] = '\0';
    node = binary_tree_node(NULL, NULL);
    root = node;
    huffqueue_add(node, NULL, &queue);
    while (queue.head && read(in_fd, tree_buffer, ARRAY_SIZE(tree_buffer)) > 0)
    {
        node = queue.head->data;

        if (tree_buffer[2] == '1')
        {
            read(in_fd, datum, 1);
            node->data = strdup(datum);
        }

        if (tree_buffer[0] == '1')
        {
            node->left = binary_tree_node(node, NULL);
            huffqueue_add(node->left, NULL, &queue);
        }
        
        if (tree_buffer[1] == '1')
        {
            node->right = binary_tree_node(node, NULL);
            huffqueue_add(node->right, NULL, &queue);
        }
        prev = queue.head;
        queue.head = queue.head->next;
        free(prev);
    }
    memset(&buffer, 0, sizeof(buffer));
    bytes = read(in_fd, &buffer, sizeof(buffer));
    while (bytes > 0)
    {
        for (i = 0; (size_t)i < sizeof(buffer); )
        {
            node = root;
            while (node->data == NULL)
            {
                if (i == sizeof(buffer))
                    i = 0, read(in_fd, &buffer, sizeof(buffer));
                if ((buffer >> (7 - i)) & 1)
                    node = node->right;
                else
                    node = node->left;
                i += 1;
            }   
            write(out_fd, node->data, 1);
            if (*((char *)(node->data)) == '\0')
                goto END;
        }
        bytes = read(in_fd, &buffer, sizeof(buffer));
    }
END:
    tree_delete(root, bah);
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

	out_fd = open(argv[3], O_WRONLY | O_CREAT | O_EXCL);
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
