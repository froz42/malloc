#include "malloc.h"
#include <stdio.h>


block_ptr split_block(block_ptr block, size_t size, void *areaend)
{
	size_t const new_size_block_a = size;
	size_t const new_size_block_b = get_block_size(block) - size;

	if (new_size_block_a < MINIMAL_SIZE || new_size_block_b < MINIMAL_SIZE)
	{
		printf("Error: block is too small for split\n");
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