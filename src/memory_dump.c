/**
 * @file memory_dump.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief this file contains tools to dump the memory in a fancy way
 * it allow to check if the memory is allocated correctly
 * @date 2022-06-06
 * 
 */

#include "malloc.h"
#include <unistd.h>


/**
 * @brief this function print the memory in a fancy way
 * an allocated block is printed in red and a free block is printed in green
 * @param area_start the start of the area
 * @param area_end the end of the area
 * @param scale the scale of the area, nbrs of block = size / scale
 */
static void dump_area(void *area_start, void *area_end, int scale)
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

/**
 * @brief Sum the size of all the blocks in the area
 * 
 * @param area_start the start of the area
 * @param area_end the end of the area
 * @return size_t the sum of the size of all the blocks in the area
 */
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

/**
 * @brief check if the sum of the blocks is equal to the size of the area
 * 
 * @param area_start the start of the area
 * @param area_end the end of the area
 * @param expected the expected size of the area
 */
static void check_sum(void *area_start, void *area_end, size_t expected)
{
	size_t sum = sum_of_block(area_start, area_end);
	if (sum != expected)
		ft_putstr("\033[31mchecksum error\033[0m\n");
	else
		ft_putstr("\033[32mchecksum ok\033[0m\n");
}


/**
 * @brief Check if the prev of each block is correct
 * 
 * @param area_start the start of the area
 * @param area_end the end of the area
 */
static void check_prev(void *area_start, void *area_end)
{
	block_ptr block = area_start;
	block_ptr prev = NULL;
	size_t i = 0;
	while (block < area_end)
	{
		block_ptr prev_ptr = *get_prev_block(block);
		if (prev_ptr != prev)
		{
			ft_putstr("\033[31mprev error\033[0m\n");
			ft_putstr("\033[31mblock: \033[0m");
			ft_putnbr(i);
			ft_putstr("\n");
			return ;
		}
		prev = block;
		block = get_next_block(block);
		i++;
	}
	ft_putstr("\033[32mprev ok\033[0m\n");
}


/**
 * @brief check if the nil node has every attribute well set
 * 
 */
static void nil_node_status(void)
{
	block_ptr nil = get_nil_node();

	if (*get_parent(nil) != nil)
		ft_putstr("\033[31mparent error\033[0m\n");
	else
		ft_putstr("\033[32mparent ok\033[0m\n");
	
	if (*get_left_child(nil) != nil)
		ft_putstr("\033[31mleft error\033[0m\n");
	else
		ft_putstr("\033[32mleft ok\033[0m\n");
	
	if (*get_right_child(nil) != nil)
		ft_putstr("\033[31mright error\033[0m\n");
	else
		ft_putstr("\033[32mright ok\033[0m\n");
	
	if (*get_color(nil) != BLACK)
		ft_putstr("\033[31mcolor error\033[0m\n");
	else
		ft_putstr("\033[32mcolor ok\033[0m\n");
}

/**
 * @brief this function count the number of free block in the area
 * 
 * @param area_start the start of the area
 * @param area_end the end of the area
 * @return size_t the number of free block in the area
 */
static size_t count_free_blocks(void *area_start, void *area_end)
{
	size_t i = 0;
	block_ptr block = area_start;
	while (block < area_end)
	{
		if (!is_allocated(block))
			i++;
		block = get_next_block(block);
	}
	return (i);
}

static void check_trees(area_ptr area)
{
	free_tree_t *tree = get_free_trees();

	size_t sum_of_tiny_tree = count_nodes(&tree->tiny);
	size_t sum_of_small_tree = count_nodes(&tree->small);

	size_t sum_of_tiny_free_blocks = count_free_blocks(area, get_small_area(area));
	size_t sum_of_small_free_blocks = count_free_blocks(get_small_area(area), get_large_area(area));

	if (sum_of_tiny_tree != sum_of_tiny_free_blocks)
		ft_putstr("\033[31mTiny tree error\033[0m\n");
	else
		ft_putstr("\033[32mTiny tree ok\033[0m\n");
	
	if (sum_of_small_tree != sum_of_small_free_blocks)
		ft_putstr("\033[31mSmall tree error\033[0m\n");
	else
		ft_putstr("\033[32mSmall tree ok\033[0m\n");
}

/**
 * @brief This function print the memory in a fancy way
 * and perform some checks on the memory
 */
void fancy_memory_dump(void)
{
	area_ptr area = get_or_create_area();
	if (area == NULL)
	{
		ft_putstr("\033[31marea error\033[0m\n");
		return ;
	}
	else
		ft_putstr("\033[32marea ok\033[0m\n");
	ft_putstr("tiny: \n");
	check_sum(area, get_small_area(area), TINY_CAPACITY);
	check_prev(area, get_small_area(area));
	dump_area(area, get_small_area(area), TINY_MAX_SIZE / 2);

	ft_putstr("small: \n");
	check_sum(get_small_area(area), get_large_area(area), SMALL_CAPACITY);
	check_prev(get_small_area(area), get_large_area(area));
	dump_area(get_small_area(area), get_large_area(area), SMALL_MAX_SIZE / 2);

	ft_putstr("nil node status: \n");
	nil_node_status();

	ft_putstr("free tree status: \n");
	check_trees(area);
}