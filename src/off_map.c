/**
 * @file off_map.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief this file contains utilities to handle off map allocation
 * each block allocated is add to a double linked list
 * @date 2022-06-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "malloc.h"
#include <sys/mman.h>


/**
 * @brief Get the off map list object
 * 
 * @return block_ptr* return the head of the list
 */
block_ptr *get_off_map_list(void)
{
	static block_ptr off_map = NULL;
	return &off_map;
}

/**
 * @brief add a block to the off map list
 * O(1) insertion
 * @param block the block to add
 */
void add_off_map(block_ptr block)
{
	block_ptr *off_map = get_off_map_list();

	block_ptr next = *off_map;
	if (next != NULL)
		*get_prev_block(next) = block;
	*get_prev_block(block) = NULL;
	*get_off_map_next_block(block) = next;
	*off_map = block;
}

/**
 * @brief remove a block from the off map list
 * O(1) removal
 * @param block the block to remove
 */
void remove_off_map(block_ptr block)
{
	block_ptr *off_map = get_off_map_list();
	block_ptr prev = *get_prev_block(block);
	block_ptr next = *get_off_map_next_block(block);

	if (prev != NULL)
		*get_off_map_next_block(prev) = next;
	if (next != NULL)
		*get_prev_block(next) = prev;
	if (*off_map == block)
		*off_map = next;
}

/**
 * @brief Get the off map count of blocks
 * 
 * @return size_t the number of blocks in the off map
 */
size_t get_off_map_size(void)
{
	block_ptr *off_map = get_off_map_list();
	size_t size = 0;
	block_ptr block = *off_map;
	while (block != NULL)
	{
		size++;
		block = *get_off_map_next_block(block);
	}
	return size;
}

/**
 * @brief Manage the allocation of an off map block
 * 
 * @param size 
 * @return block_ptr 
 */
block_ptr new_off_map_block(size_t size)
{
	block_ptr block = mmap(NULL, ALLIGN_PAGE(size + 32), PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (block == MAP_FAILED)
		return NULL;
	init_area(block, size + 16);
	add_off_map(block);
	set_allocated(block);
	return block;
}


/**
 * @brief Free an off map block
 * 
 * @param block 
 */
void remove_off_map_block(block_ptr block)
{
	set_free(block);
	remove_off_map(block);
	munmap(block, ALLIGN_PAGE(get_block_size(block) + 16));
}
