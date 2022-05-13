#include <unistd.h>
#include <sys/mman.h>
#include "malloc.h"


static void init_area(void *area, size_t size)
{
	*(size_t *)area = size;
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
	static void *area = NULL;

	if (area == NULL)
	{
		area = mmap(NULL, TOTAL_CAPACITY, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (area == MAP_FAILED)
			area = NULL;
		else
		{
			init_area(area, TINY_CAPACITY - sizeof(void *) * 2);
			init_area((char *)area + TINY_CAPACITY, SMALL_CAPACITY - sizeof(void *) * 2);
		}
	}
	return area;
}

void *ft_malloc(size_t size)
{
	void *start = get_or_create_area();
	void *block;

	if (start == NULL)
		return NULL;
	block = find_fit(start, size);
	if (block == NULL)
		return NULL;
	set_allocated(block);
	return (get_block_data(block));
}