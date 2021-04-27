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
            node = node->right, printf("RIGHT ");
        else
            node = node->left, printf("LEFT ");
        f -= 1;
    }

    if (n & 1)
    {
        printf("Inserting Right\n");
        node->right = binary_tree_node(node->right, data);
        return (node->right);
    }
    else
    {
        printf("Inserting Left\n");
        node->left = binary_tree_node(node->left, data); 
        return (node->left);
    }

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
