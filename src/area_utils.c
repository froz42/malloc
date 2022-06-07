/**
 * @file area_utils.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief this file contains the area utils functions
 * @date 2022-06-06
 *
 */

#include "malloc.h"
#include <sys/mman.h>

/**
 * @brief init the area to the given size
 * the size of the block is size - sizeof(size_t) - sizeof(void *)
 * @param area the area to init
 * @param size the size of the area
 */
void init_area(area_ptr area, size_t size)
{
	set_raw_block_size(area, size - sizeof(void *) * 2);
	*get_prev_block(area) = NULL;
}

/**
 * @brief Get the or create a area to store small and medium allocation
 * if the area is exist, return the area
 * if the area is not exist, create it and return the area
 * @return area_ptr The area created or NULL if failed
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

/**
 * @brief Find area end by where the block is
 *
 * @param area area start ptr
 * @param block block ptr
 * @return area_ptr The area end
 */
area_ptr find_area_end(area_ptr area, block_ptr block)
{
	if (block < get_small_area(area))
		return get_small_area(area);
	return get_large_area(area);
}

/**
 * @brief Handle allocation of off-map memory
 *
 * @param size size of the block
 * @return void* The allocated memory or NULL if failed
 */
void *handle_off_map(size_t size)
{
	const block_ptr new_block = new_off_map_block(size);
	if (new_block == NULL)
		return NULL;
	return get_block_data(new_block);
}

/**
 * @brief check if the block is in the area
 *
 * @param data data ptr
 * @param area area start ptr
 * @return int 1 if in the area, 0 if not
 */
int is_off_map(void *data, area_ptr area)
{
	return (data < area || data >= get_large_area(area));
}

/**
 * @brief Get the minimal size a block can have in the area
 *  undefined if off map
 * @param block the block ptr
 * @param area the area ptr
 * @return size_t the minimal size
 */
size_t get_minimal_size(block_ptr block, area_ptr area)
{
	if (block < get_small_area(area))
		return MINIMAL_SIZE;
	return TINY_MAX_SIZE + 1;
}