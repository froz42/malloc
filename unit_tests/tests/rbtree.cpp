#include "../car.hpp"

car_test tree_access(void)
{
	char block[1024];
	init_area((block_ptr)block, 1024);
	*get_left_child(block) = (void *)0xFFFFBBBB;
	*get_right_child(block) = (void *)0xFFFFCCCC;
	*get_parent(block) = (void *)0xFFFFAAAA;
	*get_color(block) = RED;

	car_assert_cmp(*get_left_child(block), (void *)0xFFFFBBBB);
	car_assert_cmp(*get_right_child(block), (void *)0xFFFFCCCC);
	car_assert_cmp(*get_parent(block), (void *)0xFFFFAAAA);
	car_assert_cmp(*get_color(block), RED);
}

car_test dummy_node(void)
{
	block_ptr nil = get_nil_node();

	car_assert_cmp(*get_left_child(nil), nil);
	car_assert_cmp(*get_right_child(nil), nil);
	car_assert_cmp(*get_parent(nil), nil);
	car_assert_cmp(*get_color(nil), BLACK);
}

car_test get_free_trees_tests(void)
{
	block_ptr nil = get_nil_node();
	free_tree_t *trees = get_free_trees();

	car_assert_cmp(trees->small, nil);
	car_assert_cmp(trees->tiny, nil);
}

car_test test_insertion(void)
{
	char block[128];
	char block2[64];

	init_area(block, 128);
	init_area(block2, 64);

	insert_free_block(block);

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();
	
	car_assert_cmp(trees->tiny, (block_ptr)block);
	car_assert_cmp(*get_parent(block), nil);
	car_assert_cmp(*get_left_child(block), nil);
	car_assert_cmp(*get_right_child(block), nil);
	car_assert_cmp(*get_color(block), BLACK);
	car_assert_cmp(trees->small, nil);

	insert_free_block(block2);

	car_assert_cmp(trees->tiny, (block_ptr)block);
	car_assert_cmp(*get_parent(block), nil);
	car_assert_cmp(*get_left_child(block), (block_ptr)block2);
	car_assert_cmp(*get_right_child(block), nil);
	car_assert_cmp(*get_color(block), BLACK);
	car_assert_cmp(*get_parent(block2), (block_ptr)block);
	car_assert_cmp(*get_left_child(block2), nil);
	car_assert_cmp(*get_right_child(block2), nil);
	car_assert_cmp(*get_color(block2), RED);

	// clear changes
	trees->tiny = nil;
	trees->small = nil;
}