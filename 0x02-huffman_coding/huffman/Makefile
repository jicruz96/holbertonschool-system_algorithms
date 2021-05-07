CC               = gcc
INCLUDE          = heap/ ./
CFLAGS           = -g3 -Wall -Werror -Wextra -pedantic $(addprefix -I../, $(INCLUDE))
HEAP_DIR_SRC     = $(addprefix /heap/, $(addprefix heap_, create.c insert.c extract.c delete.c) binary_tree_node.c)
HUFFMAN_SRC      = $(addprefix huffman_, priority_queue.c extract_and_insert.c tree.c compress.c codes.c) symbol.c binary_tree_print.c get_huffman_codes.c
SRC              = $(addprefix ../, $(HUFFMAN_SRC) $(HEAP_DIR_SRC))
FILENAME         = huffman

$(FILENAME): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $(SRC) -o $(FILENAME)
