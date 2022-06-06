/**
 * @file split_block.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file conatins utilities to handle split block and defragmentation
 * @date 2022-06-06
 * 
 */

#include "malloc.h"
#include <stdio.h>

/**
 * @brief This function split a block in two 
 * It modifies the prev of the next block
 * and take the prev of the actual block
 * @param block the block to split
 * @param size the size wanted for the block
 * @param areaend the end of the area
 * @return block_ptr the new block
 */
block_ptr split_block(block_ptr block, size_t size, void *areaend)
{
	size_t const new_size_block_a = size;
	size_t const new_size_block_b = get_block_size(block) - size;

	if (new_size_block_a < MINIMAL_SIZE || new_size_block_b < MINIMAL_SIZE)
	{
		error_write("split_block: block too small");
		return (NULL);
	}

	block_ptr const next = get_next_block(block);
	block_ptr const prev = *get_prev_block(block);

	set_raw_block_size(block, new_size_block_a);
	*get_prev_block(block) = prev;

	block_ptr const new_block = get_next_block(block);
	
	set_raw_block_size(new_block, new_size_block_b - (sizeof(void *) + sizeof(size_t)));
	*get_prev_block(new_block) = block;

	if (next < areaend)
		*get_prev_block(next) = new_block;
	return (new_block);
}

/**
 * @brief This function merge two blocks
 * it will merge the block a and the next block
 * @param block_a the first block
 * @param area_end the end of the area
 */
void merge_next_block(block_ptr block_a, void *area_end)
{
	block_ptr const block_b = get_next_block(block_a);
	block_ptr const prev = *get_prev_block(block_a);
	block_ptr const next = get_next_block(block_b);

	set_raw_block_size(block_a,
		get_block_size(block_a)
		+ get_block_size(block_b)
		+ sizeof(size_t) + sizeof(void *));
	*get_prev_block(block_a) = prev;
	if (next < area_end)
		*get_prev_block(next) = block_a;
}

/**
 * @brief This function allow to unfrag a block
 * It will check if the prev block is free and if it is the case it will merge them
 * it will also check if the next block and do the same
 * @param block the block to unfrag
 * @param area_end the end of the area
 * @param root the root of the rbtree
 * @return block_ptr the new block
 */
block_ptr unfrag_block(block_ptr block, void *area_end, block_ptr *root)
{
	if (is_allocated(block))
	{
		error_write("unfrag_block: block is allocated");
		return (NULL);
	}

	block_ptr prev = *get_prev_block(block);
	if (prev != NULL && !is_allocated(prev))
	{
		block = prev;
		delete_free_block(block, root);
		merge_next_block(block, area_end);
	}
	block_ptr next = get_next_block(block);
	if (next < area_end && !is_allocated(next))
	{
		delete_free_block(next, root);
		merge_next_block(block, area_end);
	}
	return (block);
}