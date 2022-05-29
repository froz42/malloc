#include "malloc.h"
#include <stdio.h>

block_ptr unfrag_block(block_ptr block, void *area_end, block_ptr *root)
{
	if (is_allocated(block))
	{
		printf("unfrag_block: block is allocated\n");
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