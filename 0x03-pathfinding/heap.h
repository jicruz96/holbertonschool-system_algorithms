#ifndef _HEAP_H_
#define _HEAP_H_

typedef struct heap_s
{
	void **data;
	int (*compare)(void *, void *);
	size_t data_size;
	size_t size;
} heap_t;

void *heap_pop(heap_t *heap);
void *heap_push(void *node, heap_t *heap);
void heap_sort(heap_t *heap);
void heapify(heap_t *heap, size_t i, size_t size);
heap_t *heap_init(size_t size, int (*compare)(void *, void *));
void heap_delete(heap_t *heap);
int in_heap(heap_t *heap, void *datum);

#endif /* _HEAP_H_ */
