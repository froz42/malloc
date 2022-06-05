#include <unistd.h>
#include <sys/mman.h>
#include "malloc.h"
#include <stdio.h>
#include <string.h>


void error_write(char *str)
{
	size_t len = 0;
	while (str[len] != '\0')
		len++;
	write(2, str, len);
	write(2, "\n", 1);
}

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

area_ptr get_area_end(area_ptr area, size_t size)
{
	if (size <= TINY_CAPACITY)
		return get_small_area(area);
	else if (size <= SMALL_CAPACITY)
		return get_large_area(area);
	return NULL;
}

area_ptr find_area_end(area_ptr area, block_ptr block)
{
	if (block < area)
		return NULL;
	if (block < get_small_area(area))
		return get_small_area(area);
	if (block < get_large_area(area))
		return get_large_area(area);
	return NULL;
}

void *handle_off_map(size_t size)
{
	const block_ptr new_block = new_off_map_block(size);
	if (new_block == NULL)
		return NULL;
	return get_block_data(new_block);
}

int is_off_map(void *data, area_ptr area)
{
	return (data < area || data >= get_large_area(area));
}

void *MALLOC_NAME(size_t size)
{
	size = ALLIGN_16(size);
	
	if (size < MINIMAL_SIZE)
		size = MINIMAL_SIZE;

	area_ptr area = get_or_create_area();
	if (area == NULL)
		return NULL;

	block_ptr *root = get_proper_root(size);
	
	if (!root)
		return handle_off_map(size);

	block_ptr best_fit = find_best_fit(size, root);
	if (!best_fit)
		return handle_off_map(size);

	delete_free_block(best_fit, root);
	if (get_block_size(best_fit) - size > MINIMAL_SIZE)
	{
		block_ptr new_block = split_block(best_fit, size, get_area_end(area, size));
		insert_free_block(new_block, root);
	}
	set_allocated(best_fit);
	return get_block_data(best_fit);
}

void *CALLOC_NAME(size_t nmemb, size_t size)
{
	size_t total_size = nmemb * size;
	if (total_size == 0)
		total_size = 1;
	void *data = MALLOC_NAME(total_size);
	if (data)
		memset(data, 0, total_size);
	return data;
}

void *REALLOC_NAME(void *ptr, size_t size)
{
	if (ptr == NULL)
		return MALLOC_NAME(size);
	block_ptr block = get_block_from_data(ptr);
	size_t old_size = get_block_size(block);

	if (size == 0)
	{
		FREE_NAME(ptr);
		return NULL;
	}
	if (size <= old_size)
		return ptr;
	
	void *new_ptr = MALLOC_NAME(size);
	if (new_ptr)
	{
		memcpy(new_ptr, ptr, old_size);
		FREE_NAME(ptr);
	}
	return new_ptr;
}

void FREE_NAME(void *data)
{
	if (data == NULL)
		return;


	area_ptr area = get_or_create_area();
	if (area == NULL)
		return;
	block_ptr block = get_block_from_data(data);

	if (!is_allocated(block))
	{
		error_write("free(): double free detected");
		return;
	}
	
	if (is_off_map(block, area))
		return remove_off_map_block(block);

	set_free(block);

	block_ptr *root = find_proper_root(area, block);
	if (!root)
	{
		error_write("free: failed to find root");
		return;
	}

	block_ptr new_block = unfrag_block(block, find_area_end(area, block), root);
	insert_free_block(new_block, root);
}
