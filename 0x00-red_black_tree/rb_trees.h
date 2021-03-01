#ifndef _RB_TREES_H_
#define _RB_TREES_H_

#ifndef NULL
#define NULL ((void *)0)
#endif /* NULL */

typedef unsigned long int size_t;
#define true 1
#define false 0

#define is_red(x) ((x) && (x)->color == RED)
#define is_black(x) (!is_red(x))
#define get_sister_helper(x) \
((x)->parent->left == (x) ? (x)->parent->right : (x)->parent->left)
#define get_sister(x) ((x) && (x)->parent ? get_sister_helper(x) : NULL)

#include <stdio.h>
#include <limits.h>


/**
 * enum rb_color_e - Possible color of a Red-Black tree
 *
 * @RED: 0 -> Red node
 * @BLACK: 1 -> Black node
 * @DOUBLE_BLACK: 2 -> Double-black node (used for deletion)
 */
typedef enum rb_color_e
{
	RED = 0,
	BLACK,
	DOUBLE_BLACK
} rb_color_t;

/**
 * struct rb_tree_s - Red-Black tree node structure
 *
 * @n: Integer stored in the node
 * @parent: Pointer to the parent node
 * @left: Pointer to the left child node
 * @right: Pointer to the right child node
 * @color: Color of the node (RED or BLACK)
 */
typedef struct rb_tree_s
{
	int n;
	rb_color_t color;
	struct rb_tree_s *parent;
	struct rb_tree_s *left;
	struct rb_tree_s *right;
} rb_tree_t;


rb_tree_t *rb_tree_node(rb_tree_t *parent, int value, rb_color_t color);
int rb_tree_is_valid(const rb_tree_t *tree);
rb_tree_t *array_to_rb_tree(int *array, size_t size);
rb_tree_t *rb_tree_remove(rb_tree_t *root, int n);


/* 2-rb_tree_insert.c */
rb_tree_t *rb_tree_insert(rb_tree_t **tree, int value);
void repair_case2(rb_tree_t **tree, rb_tree_t *node);
void repair_case1(rb_tree_t **tree, rb_tree_t *node);
void insert_repair(rb_tree_t **tree, rb_tree_t *node);
rb_tree_t *bst_insert(rb_tree_t **tree, int value);

/* HELPER FUNCTIONS */
rb_tree_t *get_aunt(rb_tree_t *node);
void rotate_left(rb_tree_t *node);
void rotate_right(rb_tree_t *node);

/* PLAYGROUND */
int black_node_depth(const rb_tree_t *node, int black_depth);
int check_black_node_depths(const rb_tree_t *tree);
int is_binary_search_tree(const rb_tree_t *node, int min, int max);
void rb_tree_print(const rb_tree_t *tree);

#endif /* _RB_TREES_H_ */
