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
		if (block_to_draw == 0)
			block_to_draw = 1;

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

size_t sum_of_block(void *area_start, void *area_end)
{
	size_t sum = 0;
	block_ptr block = area_start;
	while (block < area_end)
	{
		sum += get_block_size(block) + sizeof(void *) + sizeof(size_t);
		block = get_next_block(block);
	}
	return (sum);
}


void check_sum(void *area_start, void *area_end, size_t expected)
{
	size_t sum = sum_of_block(area_start, area_end);
	if (sum != expected)
		ft_putstr("\033[31mchecksum error\033[0m\n");
	else
		ft_putstr("\033[32mchecksum ok\033[0m\n");
}

void check_prev(void *area_start, void *area_end)
{
	block_ptr block = area_start;
	block_ptr prev = NULL;
	while (block < area_end)
	{
		block_ptr prev_ptr = *get_prev_block(block);
		if (prev_ptr != prev)
		{
			ft_putstr("\033[31mprev error\033[0m\n");
			return ;
		}
		prev = block;
		block = get_next_block(block);
	}
	ft_putstr("\033[32mprev ok\033[0m\n");
}

void fancy_memory_dump(void)
{
	area_ptr area = get_or_create_area();
	ft_putstr("tiny: \n");
	check_sum(area, get_small_area(area), TINY_CAPACITY);
	check_prev(area, get_small_area(area));
	dump_area(area, get_small_area(area), TINY_MAX_SIZE / 2);

	ft_putstr("small: \n");
	check_sum(get_small_area(area), get_large_area(area), SMALL_CAPACITY);
	check_prev(get_small_area(area), get_large_area(area));
	dump_area(get_small_area(area), get_large_area(area), SMALL_MAX_SIZE / 2);
}