#include <unistd.h>
#include <sys/mman.h>
#include "malloc.h"


void init_area(area_ptr area, size_t size)
{
	*(size_t *)area = size - sizeof(void *) * 2;
	*get_prev_block(area) = NULL;
}

/*
** get_or_create_area
** create a area to store small and medium allocation
** if the area is exist, return the area
** if the area is not exist, create it and return the area
** return NULL if failed
*/
void *get_or_create_area()
{
	static area_ptr area = NULL;

	if (area == NULL)
	{
		area = mmap(NULL, TOTAL_CAPACITY, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (area == MAP_FAILED)
			area = NULL;
		else
		{
			init_area(area, TINY_CAPACITY);
			init_area((char *)area + TINY_CAPACITY, SMALL_CAPACITY);
		}
	}
	return area;
}

void *ft_malloc(size_t size)
{
	area_ptr start = get_or_create_area();
	block_ptr block;

	if (start == NULL)
		return NULL;
	block = find_fit(start, size);
	if (block == NULL)
		return NULL;
	set_allocated(block);
	return (get_block_data(block));
}