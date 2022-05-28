#include <unistd.h>
#include <sys/mman.h>
#include "malloc.h"
#include <stdio.h>


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
area_ptr get_or_create_area(void)
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
			block_ptr const tiny_area = get_tiny_area(area);
			block_ptr const small_area = get_small_area(area);

			init_area(tiny_area, TINY_CAPACITY);
			init_area(small_area, SMALL_CAPACITY);

			free_tree_t *trees = get_free_trees();
			
			insert_free_block(tiny_area, &trees->tiny);
			insert_free_block(small_area, &trees->small);
		}
	}
	return area;
}

void *get_area_end(area_ptr area, size_t size)
{
	if (size <= TINY_CAPACITY)
		return get_small_area(area);
	else if (size <= SMALL_CAPACITY)
		return get_large_area(area);
	return NULL;
}

void *ft_malloc(size_t size)
{
	size = ALLIGN_16(size);
	
	if (size < MINIMAL_SIZE)
		size = MINIMAL_SIZE;

	area_ptr area = get_or_create_area();

	block_ptr *root = get_proper_root(size);
	
	if (!root)
	{
		printf("not implemented\n");
		return NULL;
	}

	block_ptr best_fit = find_best_fit(size, root);
	if (!best_fit)
	{
		printf("not implemented\n");
		return NULL;
	}

	delete_free_block(best_fit, root);
	if (get_block_size(best_fit) - size > MINIMAL_SIZE)
	{
		block_ptr new_block = split_block(best_fit, size, get_area_end(area, size));
		insert_free_block(new_block, root);
	}
	set_allocated(best_fit);
	return get_block_data(best_fit);
}