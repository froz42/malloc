#include "../car.hpp"
#include <stdio.h>

car_test tree_access(void)
{
	char block[1024];
	init_area((block_ptr)block, 1024);
	*get_left_child(block) = (void *)18446744073709551615UL;
	*get_right_child(block) = (void *)18446744073709551615UL;
	*get_parent(block) = (void *)18446744073709551615UL;
	*get_color(block) = RED;

	car_assert_cmp(*get_left_child(block), (void *)18446744073709551615UL);
	car_assert_cmp(*get_right_child(block), (void *)18446744073709551615UL);
	car_assert_cmp(*get_parent(block), (void *)18446744073709551615UL);
	
	*get_right_child(block) = (void *)0;

	car_assert_cmp(*get_left_child(block), (void *)18446744073709551615UL);
	car_assert_cmp(*get_right_child(block), (void *)0);
	car_assert_cmp(*get_parent(block), (void *)18446744073709551615UL);

	*get_left_child(block) = (void *)0;
	
	car_assert_cmp(*get_left_child(block), (void *)0);
	car_assert_cmp(*get_right_child(block), (void *)0);
	car_assert_cmp(*get_parent(block), (void *)18446744073709551615UL);
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

car_test test_insertion(void)
{
	char block[128];
	char block2[64];
	char block3[62];

	init_area(block, 128);
	init_area(block2, 64);
	init_area(block3, 62);

	free_tree_t *trees = get_free_trees();
	block_ptr *root = get_proper_root(128);
	block_ptr nil = get_nil_node();

	trees->tiny = nil;
	trees->small = nil;

	insert_free_block(block, root);

	car_assert_cmp(trees->tiny, (block_ptr)block);
	car_assert_cmp(*get_parent(block), nil);
	car_assert_cmp(*get_left_child(block), nil);
	car_assert_cmp(*get_right_child(block), nil);
	car_assert_cmp(*get_color(block), BLACK);
	car_assert_cmp(trees->small, nil);

	insert_free_block(block2, root);

	car_assert_cmp(trees->tiny, (block_ptr)block);
	car_assert_cmp(*get_parent(block), nil);
	car_assert_cmp(*get_left_child(block), (block_ptr)block2);
	car_assert_cmp(*get_right_child(block), nil);
	car_assert_cmp(*get_color(block), BLACK);
	car_assert_cmp(*get_parent(block2), (block_ptr)block);
	car_assert_cmp(*get_left_child(block2), nil);
	car_assert_cmp(*get_right_child(block2), nil);
	car_assert_cmp(*get_color(block2), RED);

	insert_free_block(block3, root);
	car_assert_cmp(trees->tiny, (block_ptr)block2);
	car_assert_cmp(*get_parent(block2), nil);
	car_assert_cmp(*get_left_child(block2), (block_ptr)block3);
	car_assert_cmp(*get_right_child(block2), (block_ptr)block);
	car_assert_cmp(*get_color(block2), BLACK);
	car_assert_cmp(*get_parent(block3), (block_ptr)block2);
	car_assert_cmp(*get_left_child(block3), nil);
	car_assert_cmp(*get_right_child(block3), nil);
	car_assert_cmp(*get_color(block3), RED);

	// clear changes
	trees->tiny = nil;
	trees->small = nil;
}

car_test test_deletion(void)
{
	char block[128];
	char block2[64];
	char block3[62];

	init_area(block, 128);
	init_area(block2, 64);
	init_area(block3, 62);

	block_ptr *root = get_proper_root(128);

	insert_free_block(block, root);
	insert_free_block(block2, root);
	insert_free_block(block3, root);

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	delete_free_block((block_ptr)block2, root);
	car_assert_cmp(trees->tiny, (block_ptr)block);
	car_assert_cmp(*get_parent(block), nil);
	car_assert_cmp(*get_left_child(block), (block_ptr)block3);
	car_assert_cmp(*get_right_child(block), nil);
	car_assert_cmp(*get_color(block), BLACK);
	car_assert_cmp(*get_parent(block3), (block_ptr)block);
	car_assert_cmp(*get_left_child(block3), nil);
	car_assert_cmp(*get_right_child(block3), nil);
	car_assert_cmp(*get_color(block3), RED);

	insert_free_block(block2, root);

	delete_free_block((block_ptr)block, root);
	car_assert_cmp(trees->tiny, (block_ptr)block2);
	car_assert_cmp(*get_parent(block2), nil);
	car_assert_cmp(*get_left_child(block2), (block_ptr)block3);
	car_assert_cmp(*get_right_child(block2), nil);
	car_assert_cmp(*get_color(block2), BLACK);
	car_assert_cmp(*get_parent(block3), (block_ptr)block2);
	car_assert_cmp(*get_left_child(block3), nil);
	car_assert_cmp(*get_right_child(block3), nil);
	car_assert_cmp(*get_color(block3), RED);

	insert_free_block(block, root);

	delete_free_block((block_ptr)block3, root);
	car_assert_cmp(trees->tiny, (block_ptr)block2);
	car_assert_cmp(*get_parent(block2), nil);
	car_assert_cmp(*get_right_child(block2), (block_ptr)block);
	car_assert_cmp(*get_left_child(block2), nil);
	car_assert_cmp(*get_color(block2), BLACK);
	car_assert_cmp(*get_parent(block), (block_ptr)block2);
	car_assert_cmp(*get_left_child(block), nil);
	car_assert_cmp(*get_right_child(block), nil);
	car_assert_cmp(*get_color(block), RED);

	delete_free_block((block_ptr)block2, root);
	delete_free_block((block_ptr)block, root);
	delete_free_block((block_ptr)block3, root);

	car_assert_cmp(trees->tiny, nil);
	car_assert_cmp(trees->small, nil);
}

car_test test_find_fit(void)
{
	char block[128];
	char block2[64];
	char block3[62];
	char block4[60];
	char block5[58];
	char block6[56];
	char block7[54];
	char block8[52];
	char block9[50];
	char block10[48];

	init_area(block, 128);
	init_area(block2, 64);
	init_area(block3, 62);
	init_area(block4, 60);
	init_area(block5, 58);
	init_area(block6, 56);
	init_area(block7, 54);
	init_area(block8, 52);
	init_area(block9, 50);
	init_area(block10, 48);

	block_ptr *root = get_proper_root(128);

	insert_free_block(block, root);
	insert_free_block(block2, root);
	insert_free_block(block3, root);
	insert_free_block(block4, root);
	insert_free_block(block5, root);
	insert_free_block(block6, root);
	insert_free_block(block7, root);
	insert_free_block(block8, root);
	insert_free_block(block9, root);
	insert_free_block(block10, root);

	car_assert_cmp(find_best_fit(112, root), (block_ptr)block);
	car_assert_cmp(find_best_fit(100, root), (block_ptr)block);
	car_assert_cmp(find_best_fit(48, root), (block_ptr)block2);
	car_assert_cmp(find_best_fit(46, root), (block_ptr)block3);
	car_assert_cmp(find_best_fit(44, root), (block_ptr)block4);
	car_assert_cmp(find_best_fit(42, root), (block_ptr)block5);
	car_assert_cmp(find_best_fit(40, root), (block_ptr)block6);
	car_assert_cmp(find_best_fit(38, root), (block_ptr)block7);
	car_assert_cmp(find_best_fit(36, root), (block_ptr)block8);
	car_assert_cmp(find_best_fit(34, root), (block_ptr)block9);
	car_assert_cmp(find_best_fit(32, root), (block_ptr)block10);

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	trees->tiny = nil;
	trees->small = nil;
}


void check_tree(block_ptr block)
{
	if (block == get_nil_node())
		return;

	check_tree(*get_left_child(block));
	check_tree(*get_right_child(block));
	
}

car_test rb_tree_stree_test(void)
{
	char block[128][128];

	
	for (int i = 0; i < 128; i++)
	{
		init_area(block[i], ALLIGN_16(i));
	}

	block_ptr *root = get_proper_root(128);

	for (int i = 0; i < 128; i++)
	{
		insert_free_block(block[i], root);
		check_tree(*root);
	}

	free_tree_t *trees = get_free_trees();
	block_ptr nil = get_nil_node();

	for (int i = 0; i < 64; i++)
	{
		delete_free_block(block[i], root);
		check_tree(*root);
	}

	for (int i = 0; i < 64; i++)
	{
		insert_free_block(block[i], root);
		check_tree(*root);
	}

	for (int i = 0; i < 128; i++)
	{
		delete_free_block(block[i], root);
		check_tree(*root);
	}



	car_assert_cmp(trees->tiny, nil);

}