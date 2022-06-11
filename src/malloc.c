/**
 * @file malloc.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief The malloc functions
 * @date 2022-06-06
 *
 */

#include <unistd.h>
#include "malloc.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Custom malloc function
 * This function is named ft_malloc or malloc
 * @param size size of the block
 * @return void* The allocated memory or NULL if failed
 */
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
	if (get_block_size(best_fit) - size > get_minimal_size(best_fit, area))
	{
		block_ptr new_block = split_block(best_fit, size, find_area_end(area, best_fit));
		insert_free_block(new_block, root);
	}
	set_allocated(best_fit);
	return get_block_data(best_fit);
}

/**
 * @brief Custom calloc function
 * This function is named ft_calloc or calloc
 * @param nmemb number of elements
 * @param size size of the element
 * @return void* The allocated memory or NULL if failed
 */
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

/**
 * @brief Custom realloc function
 * This function is named ft_realloc or realloc
 * this function try to expand the block or allocate a new block
 * and copy the data to the new block
 * @param ptr the block to reallocate
 * @param size the new size of the block
 * @return void* The allocated memory or NULL if failed
 */
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
	/*if (size == old_size)
		return ptr;*/

	/*area_ptr area = get_or_create_area();
	if (size < old_size && !shrink_block(block, size, area))
		// if we succeed to shrink the block, we return the data
		return (ptr);
	
	if (size > old_size && !extend_block(block, size, area))
		// if we succeed to extend the block, we return the data
		return (ptr);*/

	void *new_ptr = MALLOC_NAME(size);
	if (new_ptr)
	{
		memcpy(new_ptr, ptr, old_size);
		FREE_NAME(ptr);
	}
	return new_ptr;
}

/**
 * @brief Custom free function
 * This function is named ft_free or free
 * @param data the data ptr to free
 */
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
