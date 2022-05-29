#include "../car.hpp"
#include <stdio.h>

car_test test_unfrag_block(void)
{
	char area[2048];

	init_area(area, 500);
	init_area(area + 500, 500);
	init_area(area + 1000, 500);

	block_ptr block1 = (block_ptr)area;
	block_ptr block2 = (block_ptr)&area[500];
	block_ptr block3 = (block_ptr)&area[1000];

	*get_prev_block(block2) = block1;
	*get_prev_block(block3) = block2;

	block_ptr *root = get_proper_root(128);

	insert_free_block(block1, root);
	insert_free_block(block3, root);

	car_assert_cmp(get_block_size(block1), 500u - (sizeof(void *) + sizeof(size_t)));
	car_assert_cmp(get_block_size(block2), 500u - (sizeof(void *) + sizeof(size_t)));
	car_assert_cmp(get_block_size(block3), 500u - (sizeof(void *) + sizeof(size_t)));
	car_assert_cmp(get_next_block(block1), block2);
	car_assert_cmp(get_next_block(block2), block3);
	car_assert_cmp(get_next_block(block3), (block_ptr)&area[1500]);
	car_assert_cmp(*get_prev_block(block1), (block_ptr)NULL);
	car_assert_cmp(*get_prev_block(block2), block1);
	car_assert_cmp(*get_prev_block(block3), block2);

	block_ptr new_block = unfrag_block(block2, &area[1500], root);

	car_assert_cmp(get_block_size(new_block), 1500u - (sizeof(void *) + sizeof(size_t)));

	*root = get_nil_node();
}