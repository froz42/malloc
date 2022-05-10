#include <stdlib.h>
#include "malloc.h"

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
size_t get_block_size(void *block)
{
	return ((*(size_t *)block) & ~1);
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
void *get_block_data(void *block)
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
void *get_next_block(void *block)
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
void **get_prev_block(void *block)
{
	return (void **)((char *)get_block_data(block) + get_block_size(block));
}

void *get_tiny_area(void *start)
{
	return (start);
}

void *get_small_area(void *start)
{
	return ((char *)start + TINY_CAPACITY);
}

void *get_large_area(void *start)
{
	return ((char *)start + TINY_CAPACITY + SMALL_CAPACITY);
}