#include "pathfinding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static queue_t *backtrack(char **map, char **visited, int rows, int cols,
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
	char **map_copy;
	queue_t *queue;


	if (!rows || !cols || !map || !start || !target)
		return (NULL);

	map_copy = malloc(sizeof(char *) * rows);
	if (!map_copy)
		return (NULL);
	queue = malloc(sizeof(queue_t));
	if (!queue)
		return (NULL);

	for (i = 0; i < rows; i++)
		map_copy[i] = strdup(map[i]);

	backtrack(map, map_copy, rows, cols, start->x, start->y, target, queue);

	for (i = 0; i < rows; i++)
		free(map_copy[i]);

	free(map_copy);
	return (queue);
}

static queue_t *backtrack(char **map, char **visited, int rows, int cols,
			int x, int y, point_t const *target, queue_t *queue)
{
	point_t *point;

	/* invalid point */
	if (x < 0 || y < 0 || x >= cols || y >= rows || visited[y][x] == '1')
		return (NULL);

	visited[y][x] = '1';

	printf("Checking coordinates [%d, %d]\n", x, y);

	if (
		(x == target->x && y == target->y) ||
		backtrack(map, visited, rows, cols, x + 1, y, target, queue) ||
		backtrack(map, visited, rows, cols, x, y + 1, target, queue) ||
		backtrack(map, visited, rows, cols, x - 1, y, target, queue) ||
		backtrack(map, visited, rows, cols, x, y - 1, target, queue)
		)
	{
		point = malloc(sizeof(point_t));
		point->x = x;
		point->y = y;
		queue_push_front(queue, point);
		return (queue);
	}

	return (NULL);
}
