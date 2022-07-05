#include "malloc.h"

/**
 * @brief Tis function print a ptr to stdout with the format of "0xFFF0" with a minimal padding of 4
 *
 * @param ptr
 */
void putptr(void *ptr)
{
	static char const hex[] = "0123456789ABCDEF";
	char buf[sizeof(size_t) * 2 + 1];
	size_t size = (size_t)ptr;

	ft_putstr("0x");
	for (int i = sizeof(size_t) * 2 - 1; i >= 0; i--)
	{
		buf[i] = hex[size & 0xF];
		size >>= 4;
	}
	buf[sizeof(size_t) * 2] = '\0';
	ft_putstr(buf);
}

void dump_block(block_ptr block)
{
	size_t size = get_block_size(block);
	putptr(block);
	ft_putstr(" - ");
	putptr((void *)((char *)block + size));
	ft_putstr(" : ");
	ft_putnbr(size);
	ft_putstr(" bytes\n");
}

static void dump_area(area_ptr start, const area_ptr end)
{
	while (start < end)
	{
		if (is_allocated(start))
			dump_block(start);
		start = get_next_block(start);
	}
}

static void dump_larges(void **blocks)
{
	for (int i = 0; blocks[i]; i++)
		dump_block(blocks[i]);
}

void **get_sorted_ptrs(void)
{
	block_ptr block = *get_off_map_list();
	void **ptrs = _malloc(sizeof(void *) * (get_off_map_size() + 1));

	// take the second block because the first is the off map list
	if (block != NULL)
		block = *get_off_map_next_block(block);

	size_t i = 0;
	while (block != NULL)
	{
		ptrs[i] = block;
		block = *get_off_map_next_block(block);
		i++;
	}
	ptrs[i] = NULL;
	for (size_t j = 0; j < i - 1; j++)
	{
		for (size_t k = 0; k < i - j - 1; k++)
		{
			if (ptrs[k] > ptrs[k + 1])
			{
				void *tmp = ptrs[k];
				ptrs[k] = ptrs[k + 1];
				ptrs[k + 1] = tmp;
			}
		}
	}
	return ptrs;
}

/**
 * @brief This function show all allocated blocks
 * 
 */
void EXPORT show_alloc_mem(void)
{
	area_ptr start = get_or_create_area();
	area_ptr small = get_small_area(start);
	area_ptr large = get_large_area(start);

	ft_putstr("TINY : ");
	putptr(start);
	ft_putstr("\n");
	dump_area(start, small);
	ft_putstr("SMALL : ");
	putptr(small);
	ft_putstr("\n");
	dump_area(small, large);
	ft_putstr("LARGE : ");
	putptr(large);
	ft_putstr("\n");
	void **blocks = get_sorted_ptrs();
	dump_larges(blocks);
	free(blocks);
}