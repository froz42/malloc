#include "../car.hpp"

car_test test_split_block(void)
{
	char area[2048];
	init_area(area, 2048);

	car_assert_cmp(get_block_size(area), 2048 - sizeof(void *) - sizeof(size_t));

	block_ptr new_block = split_block(area, 200, &area[sizeof(area)]);

	car_assert_cmp(get_block_size(area), 200u);
	car_assert_cmp(get_block_size(new_block), 2048 - (2 * (sizeof(void *) + sizeof(size_t))) - 200);

	car_assert_cmp(get_next_block(area), new_block);
	car_assert_cmp(*get_prev_block(new_block), (block_ptr)area);
	car_assert_cmp(get_next_block(new_block), (block_ptr)&area[sizeof(area)]);

	block_ptr new_block2 = split_block(new_block, 200, &area[sizeof(area)]);

	car_assert_cmp(get_block_size(new_block), 200u);
	car_assert_cmp(get_block_size(new_block2), 2048 - (3 * (sizeof(void *) + sizeof(size_t))) - 200 - 200);
	car_assert_cmp(get_next_block(new_block), new_block2);
	car_assert_cmp(*get_prev_block(new_block2), new_block);
	car_assert_cmp(get_next_block(new_block2), (block_ptr)&area[sizeof(area)]);

	block_ptr new_block3 = split_block(area, 100, &area[sizeof(area)]);

	car_assert_cmp(get_block_size(area), 100u);
	car_assert_cmp(get_block_size(new_block3), 200 - (sizeof(void *) + sizeof(size_t)) - 100);
	car_assert_cmp(get_next_block(area), new_block3);
	car_assert_cmp(*get_prev_block(new_block3), (block_ptr)area);
	car_assert_cmp(get_next_block(new_block3), new_block);
	car_assert_cmp(*get_prev_block(new_block), new_block3);

	// check all block
	car_assert_cmp(get_block_size(area), 100u);
	car_assert_cmp(get_next_block(area), new_block3);
	car_assert_cmp(*get_prev_block(area), (block_ptr)NULL);

	car_assert_cmp(get_block_size(new_block3), 84u);
	car_assert_cmp(get_next_block(new_block3), new_block);
	car_assert_cmp(*get_prev_block(new_block3), area);

	car_assert_cmp(get_block_size(new_block), 200u);
	car_assert_cmp(get_next_block(new_block), new_block2);
	car_assert_cmp(*get_prev_block(new_block), new_block3);

	car_assert_cmp(get_block_size(new_block2), 1600u);
	car_assert_cmp(get_next_block(new_block2), (block_ptr)&area[sizeof(area)]);
	car_assert_cmp(*get_prev_block(new_block2), new_block);
}

car_test test_merge_next_block(void)
{
	char area[2048];

	init_area(area, 256);
	init_area(&area[256], 256);
	init_area(&area[512], 256);

	*get_prev_block(area) = NULL;
	*get_prev_block(&area[256]) = area;
	*get_prev_block(&area[512]) = &area[256];

	merge_next_block(&area[256], &area[512]);

	car_assert_cmp(get_block_size(&area[256]), 512u - sizeof(void *) - sizeof(size_t));
	// test prev block
	car_assert_cmp(*get_prev_block(&area[256]), (void *)area);

	merge_next_block(area, &area[512]);

	car_assert_cmp(get_block_size(area), 768u - sizeof(void *) - sizeof(size_t));
	// test prev block
	car_assert_cmp(*get_prev_block(area), (void *)NULL);
}

car_test test_extend_block(void)
{
	reset_area();

	area_ptr area = get_or_create_area();

	void *data1 = ft_malloc(112);
	block_ptr block1 = get_block_from_data(data1);

	car_assert_cmp(get_block_size(block1), 112u);

	int ret = extend_block(block1, 224, area);

	car_assert_cmp(ret, 0);

	car_assert_cmp(get_block_size(block1), 224u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	ret = extend_block(block1, TINY_MAX_SIZE, area);

	car_assert_cmp(ret, 0);

	car_assert_cmp(get_block_size(block1), (size_t)TINY_MAX_SIZE);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);
	car_assert_cmp(*get_prev_block(get_next_block(block1)), block1);

	ret = extend_block(block1, TINY_MAX_SIZE + 1, area);

	car_assert_cmp(ret, 1);
	car_assert_cmp(get_block_size(block1), (size_t)TINY_MAX_SIZE);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);
	car_assert_cmp(*get_prev_block(get_next_block(block1)), block1);

	reset_area();

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	size_t i = 0;
	void *allocs[10000];
	while (trees->tiny != nil)
	{
		allocs[i] = ft_malloc(112);
		i++;
	}

	block_ptr last_block = get_block_from_data(allocs[i - 1]);
	car_assert_cmp(get_block_size(last_block), 112u);
	car_assert(get_next_block(last_block) == find_area_end(area, last_block));

	ret = extend_block(last_block, TINY_MAX_SIZE, area);

	car_assert_cmp(ret, 1);
	car_assert_cmp(get_block_size(last_block), 112u);

	ft_free(allocs[i - 1]);

	last_block = get_block_from_data(allocs[i - 2]);
	car_assert_cmp(get_block_size(last_block), 112u);

	ret = extend_block(last_block, TINY_MAX_SIZE, area);

	car_assert_cmp(ret, 1);
	car_assert_cmp(get_block_size(last_block), 112u);

	reset_area();

	data1 = ft_malloc(112);

	block1 = get_block_from_data(data1);

	ret = extend_block(block1, 10, area);

	car_assert_cmp(ret, 1);

	car_assert_cmp(get_block_size(block1), 112u);

	ret = extend_block(block1, TINY_MAX_SIZE + 1, area);

	car_assert_cmp(ret, 1);

	reset_area();

	data1 = ft_malloc(128);
	block1 = get_block_from_data(data1);
	void *data2 = ft_malloc(128);
	ft_malloc(256);
	ft_free(data2);

	ret = extend_block(block1, 256, area);

	car_assert_cmp(ret, 0);

	car_assert_cmp(get_block_size(block1), 256u + sizeof(void *) + sizeof(size_t));

	reset_area();

	data1 = ft_malloc(128);
	block1 = get_block_from_data(data1);
	data2 = ft_malloc(128);
	ft_malloc(256);
	ft_free(data2);

	ret = extend_block(block1, 150, area);

	car_assert_cmp(ret, 0);

	car_assert_cmp(get_block_size(block1), 150u);
}

car_test test_shrink_block(void)
{
	reset_area();

	area_ptr area = get_or_create_area();

	void *data1 = ft_malloc(112);
	block_ptr block1 = get_block_from_data(data1);

	car_assert_cmp(get_block_size(block1), 112u);

	int ret = shrink_block(block1, 80, area);

	car_assert_cmp(ret, 0);

	car_assert_cmp(get_block_size(block1), 80u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	ret = shrink_block(block1, MINIMAL_SIZE - 1, area);

	car_assert_cmp(ret, 1);
	car_assert_cmp(get_block_size(block1), 80u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	ret = shrink_block(block1, 60, area);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(block1), 60u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	ret = shrink_block(block1, 60, area);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(block1), 60u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	reset_area();

	data1 = ft_malloc(256);
	block1 = get_block_from_data(data1);
	ft_malloc(256);

	ret = shrink_block(block1, 112, area);

	block_ptr next_block = get_next_block(block1);
	car_assert_cmp(get_block_size(next_block), 128u);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(block1), 112u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	ret = extend_block(block1, 256, area);

	next_block = get_next_block(block1);
	car_assert_cmp(get_block_size(next_block), 256u);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(block1), 256u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);

	ret = shrink_block(block1, 112, area);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(block1), 112u);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);


	reset_area();

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	size_t i = 0;
	void *allocs[10000];
	while (trees->tiny != nil)
	{
		allocs[i] = ft_malloc(256);
		i++;
	}

	block_ptr last_block = get_block_from_data(allocs[i - 1]);
	car_assert_cmp(get_block_size(last_block), 256u);
	car_assert(get_next_block(last_block) == find_area_end(area, last_block));

	ret = shrink_block(last_block, 128, area);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(last_block), 128u);

	ret = shrink_block(last_block, 112, area);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(last_block), 112u);

	last_block = get_block_from_data(allocs[i - 2]);

	ret = shrink_block(last_block, 112, area);

	car_assert_cmp(ret, 0);
	car_assert_cmp(get_block_size(last_block), 112u);

	while (i > 0)
	{
		ft_free(allocs[i - 1]);
		i--;
	}

	car_assert(trees->tiny == area);
	car_assert(trees->small == get_small_area(area));

	car_assert(sum_of_block(area, get_small_area(area)) == TINY_CAPACITY);
	car_assert(sum_of_block(get_small_area(area), get_large_area(area)) == SMALL_CAPACITY);	
}