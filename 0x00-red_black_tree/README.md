# 0x04 Red-Black Trees

This is the first project of the Algorithms section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we:
* learned about [Red-Black Trees](https://en.wikipedia.org/Red-black_trees), a type of balanced binary search tree data structure.
* built a set of C functions that create, manipulate and evaluate Red-Black trees.


### The Breakdown:

| File | Description |
|-|-|
| [0-rb_tree_node.c](./0-rb_tree_node.c) | `rb_tree_t *rb_tree_node(rb_tree_t *parent, int value, rb_color_t color);` </br></br> Creates a new red-black tree node with value of `value` and color of `color`, and connects it to `parent`node. This function does not verify if the user calling in the function is using it adequately. It is merely a helper function for `rb_tree_insert`. |
| [1-rb_tree_is_valid.c](./1-rb_tree_is_valid.c) | `int rb_tree_is_valid(const rb_tree_t *tree);` </br></br> Verifies if `tree` points to a valid red-black tree. |
| [2-rb_tree_insert.c](./1-rb_tree_insert.c) | `rb_tree_t *rb_tree_insert(rb_tree_t **tree, int value);` </br></br> Inserts a new node with value `value` into the red-black tree pointed to by `tree`. |
| [3-array_to_rb_tree.c](./3-array_to_rb_tree.c) | `rb_tree_t *array_to_rb_tree(int *array, size_t size);` </br></br> Creates a red-black tree from an array of integers. |
| [4-rb_tree_remove.c](./4-rb_tree_remove.c) | `rb_tree_t *rb_tree_remove(rb_tree_t *root, int n);` </br></br> Removes the node with value `n`, if it exists, from the tree pointed to by `root` |





> Written by J. to the is I.
