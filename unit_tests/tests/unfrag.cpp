#include "../car.hpp"
#include <stdio.h>

car_test test_unfrag_block(void)
{
	char area[2048];

	block_ptr block1 = (block_ptr)area;
	block_ptr block2 = (block_ptr)&area[500];
	block_ptr block3 = (block_ptr)&area[1000];
	
	block_ptr *root = get_proper_root(128);

	*root = get_nil_node();

	init_area(block1, 500);
	init_area(block2, 500);
	init_area(block3, 500);
	*get_prev_block(block2) = block1;
	*get_prev_block(block3) = block2;


	insert_free_block(block1, root);
	insert_free_block(block3, root);

	block_ptr new_block = unfrag_block(block2, &area[1500], root);

	car_assert_cmp(get_block_size(new_block), 1500u - (sizeof(void *) + sizeof(size_t)));
	// check prev
	car_assert_cmp(*get_prev_block(new_block), (void *)NULL);
	car_assert_cmp(*root, get_nil_node());

	init_area(block1, 500);
	init_area(block2, 500);
	init_area(block3, 500);
	*get_prev_block(block2) = block1;
	*get_prev_block(block3) = block2;

	insert_free_block(block1, root);
	insert_free_block(block2, root);

	new_block = unfrag_block(block3, &area[1500], root);

	car_assert_cmp(get_block_size(new_block), 1000u - (sizeof(void *) + sizeof(size_t)));
	// check prev
	car_assert_cmp(*get_prev_block(new_block), (void *)block1);
	car_assert_cmp(*root, area);

	*root = get_nil_node();
	init_area(block1, 500);
	init_area(block2, 500);
	init_area(block3, 500);
	*get_prev_block(block2) = block1;
	*get_prev_block(block3) = block2;

	insert_free_block(block2, root);
	insert_free_block(block3, root);

	new_block = unfrag_block(block1, &area[1500], root);

	car_assert_cmp(get_block_size(new_block), 1000u - (sizeof(void *) + sizeof(size_t)));
	// check prev
	car_assert_cmp(*get_prev_block(new_block), (void *)NULL);
	car_assert_cmp(*root, block3);

	*root = get_nil_node();
}