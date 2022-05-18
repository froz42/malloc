#include "../car.hpp"

car_test tree_access(void)
{
	char block[1024];
	init_area((void *)block, 1024);
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
	void *nil = get_nil_node();

	car_assert_cmp(*get_left_child(nil), nil);
	car_assert_cmp(*get_right_child(nil), nil);
	car_assert_cmp(*get_parent(nil), nil);
	car_assert_cmp(*get_color(nil), BLACK);
}

car_test get_free_trees_tests(void)
{
	void *nil = get_nil_node();
	free_tree_t *trees = get_free_trees();

	car_assert_cmp(trees->small, nil);
	car_assert_cmp(trees->tiny, nil);
}

car_test test_insertion(void)
{
	char block[64];
	char block2[32];

	init_area(block, 64);
	init_area(block2, 32);

	insert_free_block(block);

	free_tree_t *trees = get_free_trees();
	void *nil = get_nil_node();

	car_assert_cmp(trees->tiny, nil);
}