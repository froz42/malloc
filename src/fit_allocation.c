#include "malloc.h"

/*
** check if a block is allocated
** @param block: the block to check
** @return: 1 if the block is allocated, 0 if the block is free
*/
int is_allocated(void *block)
{
	return ((*(size_t *)block) & 1);
}

/*
** search_fit find a block that can fit the size
** @param area: the area to search
** @param size: the size to fit
** @param capacity: the capacity of the area
** @return: the block that can fit the size or null if not found
*/
void *search_fit(void *area, size_t size, size_t capacity)
{
	const void *end = (char *)area + capacity;
	void *block = area;

	while (block < end)
	{
		if (is_allocated(block) == 0 && get_block_size(block) >= size)
			return (block);
		block = get_next_block(block);
	}
	return (NULL);
}


/*
** find fit determine witch area to search
** @param size: the size to fit
** @return the block that fit the size or NULL if not found
*/
void *find_fit(void *start, size_t size)
{
	if (size <= TINY_MAX_SIZE)
		return (search_fit(get_tiny_area(start), size, TINY_CAPACITY));
	else if (size <= SMALL_MAX_SIZE)
		return (search_fit(get_small_area(start), size, SMALL_CAPACITY));
	return NULL;
}