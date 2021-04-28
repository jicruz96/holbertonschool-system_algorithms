#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

int get_num_steps(int size)
{
    int steps = 0;
    int n = 4;

    while (size >= n)
    {
        steps += 1;
        n *= 2;
    }

    return (steps);
}

binary_tree_node_t *insert_last(heap_t *heap, void *data)
{
    int n = heap->size + 1;
    int f = get_num_steps(n);
    binary_tree_node_t *node;

    heap->size += 1;
    node = heap->root;

    while (f)
    {
        if (n & (1 << f))
            node = node->right;
        else
            node = node->left;
        f -= 1;
    }

    if (n & 1)
    {
        node->right = binary_tree_node(node, data);
        return (node->right);
    }
    else
    {
        node->left = binary_tree_node(node, data); 
        return (node->left);
    }

}

/**
 * swap_nodes - swaps two nodes
 * @n1: node 1
 * @n2: node 2
 **/
void swap_nodes(binary_tree_node_t *n1, binary_tree_node_t *n2)
{
  binary_tree_node_t *tmp;

  if (n1 == NULL || n2 == NULL)
    return;

  /* node1->parent->pointer */
  if (n1->parent && n1->parent != n2)
  {
    if (n1->parent->right == n1)
      n1->parent->right = n2;
    else
      n1->parent->left = n2;
  }

  /* node2->parent->pointer */
  if (n2->parent && n2->parent != n1)
  {
    if (n2->parent->right == n2)
      n2->parent->right = n1;
    else
      n2->parent->left = n1;
  }
  
  /* node1->parent */
  tmp = n1->parent;
  if (n2->parent != n1)
    n1->parent = n2->parent;
  else
    n1->parent = n2;

  /* node2->parent */
  if (tmp != n2)
    n2->parent = tmp;
  else
    n2->parent = n1;

  /* node1->right->parent */
  if (n1->right && n1->right != n2)
    n1->right->parent = n2;
  
  /* node2->right->parent */
  if (n2->right && n2->right != n1)
    n2->right->parent = n1;

  /* node1->left->parent */
  if (n1->left && n1->left != n2)
    n1->left->parent = n2;

  /* node2->left->parent */
  if (n2->left && n2->left != n2)
    n2->left->parent = n1;

  /* node1->right */
  tmp = n1->right;
  if (n1 == n2->right)
    n1->right = n2;
  else
    n1->right = n2->right;

  /* node2->right */
  if (n2 == tmp)
    n2->right = n1;
  else
    n2->right = tmp;

  /* node1->left */
  tmp = n1->left;
  if (n1 == n2->left)
    n1->left = n2;
  else
    n1->left = n2->left;

  /* node2->left */
  if (n2 == tmp)
    n2->left = n1;
  else
    n2->left = tmp;

  printf("swapped\n");
}

/**
 * heap_insert - inserts a new value into a min binary heap
 * @heap: pointer to min heap
 * @data: pointer to new data to add to heap
 * Return: pointer to new node 
 **/
binary_tree_node_t *heap_insert(heap_t *heap, void *data)
{
    binary_tree_node_t *node;

    if (heap == NULL || data == NULL)
        return (NULL);

    if (heap->root == NULL)
    {
        heap->root = binary_tree_node(NULL, data);
        heap->size = 1;
        return (heap->root);
    }

    node = insert_last(heap, data);
    while (node->parent && heap->data_cmp(node->data, node->parent->data) < 0)
      swap_nodes(node, node->parent);
    if (node->parent == NULL)
      heap->root = node;
    else
      printf("node is %d and node parent is %d\n", *(int *)(node->data), *(int *)(node->parent->data));
    return (node);
}

 /*
 To add an element to a heap, we can perform this algorithm:

Add the element to the bottom level of the heap at the leftmost open space.
Compare the added element with its parent; if they are in the correct order, stop.
If not, swap the element with its parent and return to the previous step.
The size of heap must be incremented if the function succeeds
If heap->root is NULL, then the new node must take its place
You are allowed to have at most 7 functions in your file
*/
