#include "malloc.h"
#include <sys/mman.h>

block_ptr *get_off_map_list(void)
{
	static block_ptr off_map = NULL;
	return &off_map;
}

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

block_ptr new_off_map_block(size_t size, void *area_end)
{
	block_ptr block = mmap(area_end, size + 32, PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (block == MAP_FAILED)
		return NULL;
	init_area(block, size + 16);
	add_off_map(block);
	set_allocated(block);
	return block;
}
