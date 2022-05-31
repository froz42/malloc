#include <unistd.h>
#include <sys/mman.h>
#include "malloc.h"
#include <stdio.h>
#include <string.h>


void init_area(area_ptr area, size_t size)
{
	set_raw_block_size(area, size - sizeof(void *) * 2);
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

void *handle_off_map(size_t size, void *area_end)
{
	const block_ptr new_block = new_off_map_block(size, area_end);
	if (new_block == NULL)
		return NULL;
	return get_block_data(new_block);
}

int is_off_map(void *data, area_ptr area)
{
	return (data < area || data >= get_large_area(area));
}

void *malloc(size_t size)
{
	size = ALLIGN_16(size);
	
	if (size < MINIMAL_SIZE)
		size = MINIMAL_SIZE;

	write(1, "malloc\n", 7);

	area_ptr area = get_or_create_area();

	block_ptr *root = get_proper_root(size);
	
	if (!root)
		return handle_off_map(size, get_large_area(area));

	block_ptr best_fit = find_best_fit(size, root);
	if (!best_fit)
		return handle_off_map(size, get_large_area(area));

	delete_free_block(best_fit, root);
	if (get_block_size(best_fit) - size > MINIMAL_SIZE)
	{
		block_ptr new_block = split_block(best_fit, size, get_area_end(area, size));
		insert_free_block(new_block, root);
	}
	set_allocated(best_fit);
	return get_block_data(best_fit);
}

void free(void *data)
{
	if (data == NULL)
		return;

	area_ptr area = get_or_create_area();
	block_ptr block = get_block_from_data(data);

	if (!is_allocated(block))
		return;
	
	if (is_off_map(block, area))
		return remove_off_map_block(block);

	set_free(block);

	block_ptr *root = get_proper_root(get_block_size(block));

	block_ptr new_block = unfrag_block(block, get_large_area(area), root);
	insert_free_block(new_block, root);
}
