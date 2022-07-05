/**
 * @file data_access.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief The block's data access functions
 * The block has the following structure:
 * ---------------------------------
 * | size | prev | data + padding |
 * ---------------------------------
 * size: the size of the block in bytes
 * prev: the previous block in the area
 * data + padding: the data of the block
 * The data of the block is aligned to 16 bytes 
 * 
 * The off map block has the following structure:
 * ----------------------------------------
 * | size | prev | data + padding | next |
 * ----------------------------------------
 * size: the size of the block in bytes
 * prev: the previous block in the area
 * data + padding: the data of the block
 * next: the next block in the area (only for off map blocks)
 * 
 * The rbtree is a red-black tree.
 * It is used to store free blocks and to find the best fit block.
 * The rbtree is implemented as a binary tree.
 * The nil node points to itself.
 * the block structure is as follows:
 * ----------------------------------------------------------
 * | size | prev | left | right | parent | padding |  color |
 * ----------------------------------------------------------
 * The size field is the size of the block.
 * The prev field points to the previous block.
 * The left field points to the left child.
 * The right field points to the right child.
 * The parent field points to the parent node.
 * The color field is used to indicate the color of the node.
 * The color field is either RED or BLACK.
 * @date 2022-06-06
 * 
 */

#include <stdlib.h>
#include "malloc.h"
#include <stdio.h>

/**
 * @brief Get the block size in bytes, ignore the the allocation flag
 * 
 * @param block The block
 * @return size_t The block size
 */
size_t get_block_size(block_ptr block)
{
	return ((*(size_t *)block) & ~1);
}

/**
 * @brief Set the block size in bytes, ignore the the allocation flag
 * 
 * @param block The block
 * @param size The block size
 */
void set_block_size(block_ptr block, size_t size)
{
	int const allocated = is_allocated(block);
	*(size_t *)block = size | (allocated & 1);
}


/**
 * @brief Set the raw block size in bytes, override the allocation flag
 * 
 * @param block 
 * @param size 
 */
void set_raw_block_size(block_ptr block, size_t size)
{
	*(size_t *)block = size;
}

/**
 * @brief Get the block data pointer
 * 
 * @param block The block 
 * @return void* The block data pointer
 */
void *get_block_data(block_ptr block)
{
	return ((char *)block + sizeof(size_t) + sizeof(void *));
}

/**
 * @brief Get the next block
 * Get the next block, use with caution, it does not check
 * if the next block is valid
 * 
 * @param block The block
 * @return block_ptr The next block
 */
block_ptr get_next_block(block_ptr block)
{
	return ((char *)get_block_data(block)+ get_block_size(block));
}

/**
 * @brief Get the prev block object
 * return null if the block is the first block
 * 
 * @param block The block
 * @return block_ptr* the ptr to the prev block
 */
block_ptr *get_prev_block(block_ptr block)
{
	return (void **)((char *)block + sizeof(size_t));
}

/**

 */

/**
 * @brief Get the off map next block
 * Only used in the off map area, return null if the block is the last block
 *  if used in the other area, it will segfault
 * @param block The block
 * @return block_ptr* The ptr to off map next block
 */
block_ptr *get_off_map_next_block(block_ptr block)
{
	return (void **)((char *)get_block_data(block) + get_block_size(block));
}

/**
 
 */


/**
 * @brief Get the left child of the block
 * Undefined if the block is not in a tree
 * @param block The block node
 * @return block_ptr* The ptr to left child
 */
block_ptr *get_left_child(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_left_child: block is allocated");
		return NULL;
	}
	return (get_block_data(block));
}

/**
 * @brief Get the right child of the block
 *  Undefined if the block is not in a tree
 * @param block The block node
 * @return block_ptr* The ptr to right child
 */
block_ptr *get_right_child(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_right_child: block is allocated\n");
		return NULL;
	}
	return (void **)((char *)get_block_data(block) + sizeof(void *));
}

/**
 * @brief Get the parent of the block
 * Undefined if the block is not in a tree
 * @param block 
 * @return block_ptr* The ptr to parent
 */
block_ptr *get_parent(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_parent: block is allocated");
		return NULL;
	}
	return (void **)((char *)get_block_data(block) + sizeof(void *) * 2);
}

/**
 * @brief Get the color of the block
 * Undefined if the block is not in a tree
 * @param block 
 * @return int* The ptr to color
 */
int *get_color(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_color: block is allocated");
		return NULL;
	}
	return (int *)((char *)get_block_data(block) + sizeof(void *) * 3);
}

/**
 * @brief Get the tiny area
 * 
 * @param start The start of the area
 * @return area_ptr The tiny area
 */
area_ptr get_tiny_area(void *start)
{
	return (start);
}

/**
 * @brief Get the small area
 * 
 * @param start The start of the area
 * @return area_ptr The small area
 */
area_ptr get_small_area(void *start)
{
	return (&((char *)start)[TINY_CAPACITY]);
}

/**
 * @brief Get the large area
 * everything beyond the large area is off map
 * @param start The start of the area
 * @return area_ptr The large area
 */
area_ptr get_large_area(void *start)
{
	return (&((char *)start)[TINY_CAPACITY + SMALL_CAPACITY]);
}

/**
 * @brief check if a block is allocated
 * 
 * @param block The block
 * @return int 1 if allocated, 0 if not
 */
int is_allocated(block_ptr block)
{
	return ((*(size_t *)block) & 1);
}

/**
 * @brief Set allocated flag to 1
 * 
 * @param block The block
 */
void set_allocated(block_ptr block)
{
	*(size_t *)block |= 1;
}

/**
 * @brief Set the allocated flag to 0
 * 
 * @param block The block
 */
void set_free(block_ptr block)
{
	*(size_t *)block &= ~1;
}

/**
 * @brief Get the block from data ptr
 * 
 * @param data The data ptr
 * @return block_ptr The block
 */
block_ptr get_block_from_data(void *data)
{
	return ((block_ptr)((char *)data - sizeof(size_t) - sizeof(void *)));
}