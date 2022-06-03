#include <stdlib.h>
#include "malloc.h"
#include <stdio.h>

/*
** allocated block looks like this:
** |-------------------|
** | size | data | prev|
** |-------------------|
*/


/*
** get block size
** |-------------------|
** | size | data | prev|
** |-------------------|
**  ^
**  |
** size_t & ~1
*/
size_t get_block_size(block_ptr block)
{
	return ((*(size_t *)block) & ~1);
}

void set_block_size(block_ptr block, size_t size)
{
	int const allocated = is_allocated(block);
	*(size_t *)block = size | (allocated & 1);
}

void set_raw_block_size(block_ptr block, size_t size)
{
	*(size_t *)block = size;
}

/*
** get block data
** |-------------------|
** | size | data | prev|
** |-------------------|
**         ^
**         |
** block as char * + sizeof(size_t)
*/
void *get_block_data(block_ptr block)
{
	return ((char *)block + sizeof(size_t));
}

/*
** get next block
** |-------------------|
** | size | data | prev|
** |-------------------|
**                      ^
**                      |
** get the block after the block
*/
block_ptr get_next_block(block_ptr block)
{
	return (
		(char *)get_block_data(block)
		+ get_block_size(block)
		+ sizeof(void *));
}

/*
** get previous block
** |-------------------|
** | size | data | prev|
** |-------------------|
**                ^
**                |
** get the block behind prev pointer
*/
block_ptr *get_prev_block(block_ptr block)
{
	return (void **)((char *)get_block_data(block) + get_block_size(block));
}

block_ptr *get_off_map_next_block(block_ptr block)
{
	return (void **)((char *)get_block_data(block) + get_block_size(block) + sizeof(void *));
}


/*
** get the left child of the block node
** |--------------------------------------------|
** | size | left | right| parent | color | prev |
** |--------------------------------------------|
*/
block_ptr *get_left_child(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_left_child: block is allocated");
		return NULL;
	}
	return (block_ptr *)((char *)block + sizeof(size_t));
}

/*
** get the right child of the block node
** |--------------------------------------------|
** | size | left | right| parent | color | prev |
** |--------------------------------------------|
*/
block_ptr *get_right_child(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_right_child: block is allocated\n");
		return NULL;
	}
	return (block_ptr *)((char *)block + sizeof(size_t) + sizeof(void *));
}

/*
** get the parent of the block node
** |--------------------------------------------|
** | size | left | right| parent | color | prev |
** |--------------------------------------------|
*/
block_ptr *get_parent(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_parent: block is allocated");
		return NULL;
	}
	return (block_ptr *)((char *)block + sizeof(size_t) + sizeof(void *) * 2);
}

/*
** get the color of the block node
** |--------------------------------------------|
** | size | left | right| parent | color | prev |
** |--------------------------------------------|
*/
int *get_color(block_ptr block)
{
	if (is_allocated(block))
	{
		error_write("get_color: block is allocated");
		return NULL;
	}
	return (int *)((char *)block + sizeof(size_t) + sizeof(void *) * 3);
}

area_ptr get_tiny_area(void *start)
{
	return (start);
}
area_ptr get_small_area(void *start)
{
	return (&((char *)start)[TINY_CAPACITY]);
}

area_ptr get_large_area(void *start)
{
	return (&((char *)start)[TINY_CAPACITY + SMALL_CAPACITY]);
}

/*
** check if a block is allocated
** @param block: the block to check
** @return: 1 if the block is allocated, 0 if the block is free
*/
int is_allocated(block_ptr block)
{
	return ((*(size_t *)block) & 1);
}

/*
** set a block as allocated
** @param block: the block to set
*/
void set_allocated(block_ptr block)
{
	*(size_t *)block |= 1;
}

void set_free(block_ptr block)
{
	*(size_t *)block &= ~1;
}

block_ptr get_block_from_data(void *data)
{
	return ((block_ptr)((char *)data - sizeof(size_t)));
}