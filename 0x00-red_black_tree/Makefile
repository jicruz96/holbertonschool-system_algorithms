CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic
FILENAME = hreadelf
SRC      = open_ELF.c read_elf_header_MSB.c



0_SRC = 0-main.c 0-rb_tree_node.c print_rb_tree.c
0-rb_tree_node: $(0_SRC:.c=.o)
	$(CC) $(CFLAGS) $(0_SRC) -o 0-rb_tree_node

1_SRC = 1-main.c 0-rb_tree_node.c 1-rb_tree_is_valid.c print_rb_tree.c
1-rb_tree_is_valid: $(1_SRC:.c=.o)
	$(CC) $(CFLAGS) $(1_SRC) -o 1-rb_tree_is_valid

2_SRC = 2-main.c 0-rb_tree_node.c 2-rb_tree_insert.c print_rb_tree.c
2-rb_tree_insert: $(2_SRC:.c=.o)
	$(CC) $(CFLAGS) $(2_SRC) -o 2-rb_tree_insert

3_SRC = 3-main.c 0-rb_tree_node.c 2-rb_tree_insert.c 3-array_to_rb_tree.c print_rb_tree.c
3-array_to_rb_tree: $(3_SRC:.c=.o)
	$(CC) $(CFLAGS) $(3_SRC) -o 3-array_to_rb_tree

4_SRC = 4-main.c 0-rb_tree_node.c 4-rb_tree_remove.c print_rb_tree.c
4-rb_tree_remove: $(4_SRC:.c=.o)
	$(CC) $(CFLAGS) $(4_SRC) -o 4-rb_tree_remove
