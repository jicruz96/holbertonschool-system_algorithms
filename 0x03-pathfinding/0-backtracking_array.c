#include "pathfinding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static queue_t *backtrack(char **map, char **seen, int rows, int cols,
			int x, int y, point_t const *target, queue_t *queue);

/**
 * backtracking_array - searches for the first path from a starting point to a
 *                      target point within a 2D array.
 * @map: pointer to a read-only 2D array where 1 = walkable, 2 = blocked
 * @rows: number of rows of map
 * @cols: number of columns of map
 * @start: stores the coordinates of the starting point
 * @target: stores the coordinates of the target point
 * Return: a queue of points from start to target
*/
queue_t *backtracking_array(char **map, int rows, int cols,
			point_t const *start, point_t const *target)
{
	int i;
	char **seen;
	queue_t *queue;


	if
	(
		!rows || !cols || !map || !start || !target ||
		!(queue = queue_create()) ||
		!(seen = malloc(sizeof(char *) * rows))
	)
	{
		queue_delete(queue);
		free(seen);
		return (NULL);
	}

	for (i = 0; i < rows; i++)
		seen[i] = strdup(map[i]);

	if (!backtrack(map, seen, rows, cols, start->x, start->y, target, queue))
		queue_delete(queue), queue = NULL;

	for (i = 0; i < rows; i++)
		free(seen[i]);

	free(seen);
	return (queue);
}

/**
 * backtrack - helper backtracking algorithm
 *
 * @map: pointer to 2d array map representation
 * @seen: pointer to 2d array that keeps track of seen cells
 * @rows: number of rows in map (and seen)
 * @cols: number of columns in map (and seen)
 * @x: current x position
 * @y: current y position
 * @target: target x, y position
 * @queue: pointer to queue where final path will be stored.
 * Return: pointer to queue
 */
static queue_t *backtrack(char **map, char **seen, int rows, int cols,
			int x, int y, point_t const *target, queue_t *queue)
{
	point_t *point;

	/* invalid point */
	if (x < 0 || y < 0 || x >= cols || y >= rows || seen[y][x] == '1')
		return (NULL);

	seen[y][x] = '1';

	printf("Checking coordinates [%d, %d]\n", x, y);

	if (
		(x == target->x && y == target->y) ||
		backtrack(map, seen, rows, cols, x + 1, y, target, queue) ||
		backtrack(map, seen, rows, cols, x, y + 1, target, queue) ||
		backtrack(map, seen, rows, cols, x - 1, y, target, queue) ||
		backtrack(map, seen, rows, cols, x, y - 1, target, queue)
		)
	{
		point = malloc(sizeof(point_t));
		if (!point)
		{
			queue_delete(queue);
			return (NULL);
		}
		point->x = x;
		point->y = y;
		queue_push_front(queue, point);
		return (queue);
	}

	return (NULL);
}
