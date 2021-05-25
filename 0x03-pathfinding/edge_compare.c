#include "graphs.h"


/**
 * edge_compare - comparison function for heap of edge_t objects
 * @a: first edge
 * @b: second edge
 * Return: 1 if a is greater than b | 0 otherwise
 **/
int edge_compare(void *a, void *b)
{
	return (((edge_t *)a)->weight > ((edge_t *)b)->weight);
}
