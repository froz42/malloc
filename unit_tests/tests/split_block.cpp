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