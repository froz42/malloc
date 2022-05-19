#include "malloc.h"

/*
** check if a block is allocated
** @param block: the block to check
** @return: 1 if the block is allocated, 0 if the block is free
*/
int is_allocated(block_ptr block)
{
	return ((*(size_t *)block) & 1);
}

/*
** set a block as allocated
** @param block: the block to set
*/
void set_allocated(block_ptr block)
{
	*(size_t *)block |= 1;
}

/*
** search_fit find a block that can fit the size
** @param area: the area to search
** @param size: the size to fit
** @param capacity: the capacity of the area
** @return: the block that can fit the size or null if not found
*/
block_ptr search_fit(area_ptr area, size_t size, size_t capacity)
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
block_ptr find_fit(area_ptr area, size_t size)
{
	if (size <= TINY_MAX_SIZE)
		return (search_fit(get_tiny_area(area), size, TINY_CAPACITY));
	else if (size <= SMALL_MAX_SIZE)
		return (search_fit(get_small_area(area), size, SMALL_CAPACITY));
	return NULL;
}