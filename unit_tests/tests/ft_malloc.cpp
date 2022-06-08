#include "../car.hpp"
#include <stdio.h>
#include <string.h>

void reset_area(void)
{
	area_ptr area = get_or_create_area();

	block_ptr const tiny_area = get_tiny_area(area);
	block_ptr const small_area = get_small_area(area);

	init_area(tiny_area, TINY_CAPACITY);
	init_area(small_area, SMALL_CAPACITY);

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	trees->tiny = nil;
	trees->small = nil;

	insert_free_block(tiny_area, &trees->tiny);
	insert_free_block(small_area, &trees->small);
}

car_test test_malloc(void)
{
	reset_area();

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	void *alloc1 = ft_malloc(50);

	size_t addr = (size_t)alloc1;

	// test if addr is alligned to 16
	car_assert_cmp(addr % 16, 0u);

	car_assert(alloc1 != NULL);

	block_ptr block1 = get_block_from_data(alloc1);

	car_assert_cmp(get_block_size(block1), 64u);

	car_assert_cmp(is_allocated(block1), true);

	void *alloc2 = ft_malloc(50);

	car_assert(alloc2 != NULL);

	block_ptr block2 = get_block_from_data(alloc2);

	car_assert_cmp(get_block_size(block2), 64u);

	car_assert_cmp(is_allocated(block2), true);

	car_assert_cmp(get_next_block(block1), block2);

	car_assert_cmp(*get_prev_block(block2), block1);

	area_ptr area = get_or_create_area();

	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);

	trees->tiny = nil;
	trees->small = nil;
}

car_test test_off_heap(void)
{
	reset_area();

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	void *alloc1 = ft_malloc(SMALL_MAX_SIZE + 1);

	car_assert(alloc1 != NULL);

	void *alloc2 = ft_malloc(SMALL_MAX_SIZE * 2);

	car_assert(alloc2 != NULL);

	block_ptr block1 = get_block_from_data(alloc1);
	block_ptr block2 = get_block_from_data(alloc2);

	car_assert_cmp(get_block_size(block1), ALLIGN_16(SMALL_MAX_SIZE + 1u));
	car_assert_cmp(get_block_size(block2), ALLIGN_16(SMALL_MAX_SIZE * 2u));

	area_ptr area = get_or_create_area();

	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);

	trees->tiny = nil;
	trees->small = nil;
}

car_test test_free(void)
{
	reset_area();

	area_ptr area = get_or_create_area();

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	void *alloc1 = ft_malloc(50);

	block_ptr block1 = get_block_from_data(alloc1);

	car_assert_cmp(area, block1);

	car_assert(alloc1 != NULL);
	ft_free(alloc1);

	car_assert_cmp(trees->tiny, area);

	void *alloc2 = ft_malloc(50);
	void *alloc3 = ft_malloc(50);
	void *alloc4 = ft_malloc(50);

	car_assert(alloc2 != NULL);
	car_assert(alloc3 != NULL);
	car_assert(alloc4 != NULL);

	ft_free(alloc2);
	ft_free(alloc3);
	ft_free(alloc4);

	car_assert_cmp(trees->tiny, area);
	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);

	trees->tiny = nil;
	trees->small = nil;
}

car_test test_off_map_free(void)
{
	reset_area();

	void *alloc1 = ft_malloc(SMALL_MAX_SIZE * 2);

	car_assert(alloc1 != NULL);

	block_ptr block1 = get_block_from_data(alloc1);

	area_ptr area = get_or_create_area();

	car_assert(area != block1);
	car_assert(get_small_area(area) != block1);

	ft_free(alloc1);
	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);
}

car_test tiny_alloc_stress_test(void)
{
	reset_area();

	free_tree_t *trees = get_free_trees();

	area_ptr area = get_or_create_area();

	void *alloc[300];

	for (int i = 0; i < 300; i++)
	{
		alloc[i] = ft_malloc(TINY_MAX_SIZE);
		memset(alloc[i], 0, TINY_MAX_SIZE);
	}

	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);

	car_assert(trees->tiny == get_nil_node());

	for (int i = 0; i < 300; i++)
		ft_free(alloc[i]);

	car_assert(trees->tiny == area);

	for (int i = 0; i < 300; i++)
	{
		alloc[i] = ft_malloc(TINY_MAX_SIZE);
		memset(alloc[i], 0xFF, TINY_MAX_SIZE);
	}

	for (int i = 0; i < 300; i++)
		ft_free(alloc[i]);

	car_assert(trees->tiny == area);
	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);
}

car_test small_alloc_stress_test(void)
{
	reset_area();

	void *alloc[300];

	for (int i = 0; i < 300; i++)
	{
		alloc[i] = ft_malloc(SMALL_MAX_SIZE);
		memset(alloc[i], 0xFF, SMALL_MAX_SIZE);
	}

	for (int i = 0; i < 300; i++)
		ft_free(alloc[i]);

	area_ptr area = get_or_create_area();
	free_tree_t *trees = get_free_trees();

	car_assert(trees->small == get_small_area(area));
	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);
}

car_test stress_test_mixed(void)
{
	reset_area();

	void *allocs_tiny[300];
	void *allocs_small[300];

	for (int i = 0; i < 256; i++)
	{
		allocs_tiny[i] = ft_malloc(TINY_MAX_SIZE);
		memset(allocs_tiny[i], 0xFF, TINY_MAX_SIZE);

		allocs_small[i] = ft_malloc(SMALL_MAX_SIZE);
		memset(allocs_small[i], 0xFF, SMALL_MAX_SIZE);
	}

	int i;
	for (i = 0; i < 255; i++)
	{
		ft_free(allocs_tiny[i]);
		ft_free(allocs_small[i]);
	}

	ft_free(allocs_tiny[i]);
	ft_free(allocs_small[i]);
	area_ptr area = get_or_create_area();
	free_tree_t *trees = get_free_trees();

	car_assert(trees->tiny == area);
	car_assert(trees->small == get_small_area(area));

	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);
}


car_test spam_minimal_size(void)
{
	reset_area();

	void *alloc_table[10000];

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	size_t i = 0;
	while (trees->tiny != nil)
	{
		alloc_table[i] = ft_malloc(1);
		i++;
	}

	fancy_memory_dump();

	for (size_t y = 0; y < i; y++)
		ft_free(alloc_table[y]);

	fancy_memory_dump();
}

car_test spam_minimal_size_2(void)
{
	reset_area();

	void *alloc_table[10000];

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	size_t i = 0;
	while (trees->small != nil)
	{
		alloc_table[i] = ft_malloc(TINY_MAX_SIZE + 1);
		i++;
	}
	
	fancy_memory_dump();

	for (size_t y = 0; y < i; y++)
		ft_free(alloc_table[y]);

	fancy_memory_dump();
}