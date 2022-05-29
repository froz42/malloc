#include "../car.hpp"
#include <stdio.h>

void reset_area(void)
{
	area_ptr area = get_or_create_area();

	printf("size of area: %zu\n", get_block_size(area));


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

car_test test_ft_malloc(void)
{
	reset_area();

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	void *alloc1 = ft_malloc(50);

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

	trees->tiny = nil;
	trees->small = nil;
}


void test_off_heap(void)
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

	trees->tiny = nil;
	trees->small = nil;
}

car_test test_ft_free(void)
{
	reset_area();

	area_ptr area = get_or_create_area();


	printf("size of area: %zu\n", get_block_size(area));
	free_tree_t *trees = get_free_trees();

	printf("size of tiny block: %zu\n", get_block_size(trees->tiny));


	void *alloc1 = ft_malloc(50);



	block_ptr block1 = get_block_from_data(alloc1);

	car_assert_cmp(area, block1);

	car_assert(alloc1 != NULL);
	//ft_free(alloc1);
}