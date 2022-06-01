#include "malloc.h"
#include <unistd.h>

void ft_putstr(char *str)
{
	size_t i = 0;
	while (str[i])
		i++;
	write(1, str, i);
}

void dump_area(void *area_start, void *area_end, int scale)
{
	block_ptr block = area_start;
	while (block < area_end)
	{
		const size_t size = get_block_size(block);
		const int allocated = is_allocated(block);
		size_t block_to_draw = size / scale;

		if (allocated)
			ft_putstr("\033[31m");
		else
			ft_putstr("\033[32m");

		while (block_to_draw--)
		{
			ft_putstr("▀");
		}

		ft_putstr(" \033[0m");

		block = get_next_block(block);
	}
	ft_putstr("\n");
}

void fancy_memory_dump(void)
{
	area_ptr area = get_or_create_area();
	ft_putstr("tiny: ");
	dump_area(area, get_small_area(area), 100);

	ft_putstr("small: ");
	dump_area(get_small_area(area), get_large_area(area), 500);

}