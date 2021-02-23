#include "rb_trees.h"

/**
 * rb_tree_is_valid - checks if a tree structure is a valid Red-Black Tree
 * @tree: pointer to root of tree
 * Return: 1 if valid Red-Black Tree | 0 otherwise
 **/
int rb_tree_is_valid(const rb_tree_t *tree)
{
    if (!tree || tree->color != BLACK)
        return (0);

    return (1);
}

/*
Write a function that checks if a binary tree is a valid Red-Black Tree

Prototype: int rb_tree_is_valid(const rb_tree_t *tree);
Where tree is a pointer to the root node of the tree to check
Your function must return 1 if tree is a valid Red-Black Tree, and 0 otherwise
If tree is NULL, return 0
Properties of a Red-Black Tree:

A Red-Black Tree is a BST
Every node has a color either red or black.
Root of tree is always black.
There are no two adjacent red nodes (A red node cannot have a red parent or red child).
Every path from root to a NULL node has same number of black nodes.
*/
