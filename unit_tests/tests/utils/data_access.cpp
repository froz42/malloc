#include "../../car.hpp"

car_test data_access(void)
{
	char block[1024];
	void *block_ptr = block;
	size_t *head = (size_t *)block_ptr;
	*head = 64;
	void *data = (char *)head + sizeof(size_t);
	void *next = (char *)data + 64 + sizeof(void *);
	void **prev = (void **)((char *)data + 64);
	*prev = NULL;

	car_assert_cmp(get_block_size(block_ptr), 64U);
	car_assert_cmp(get_block_data(block_ptr), data);
	car_assert_cmp(get_next_block(block_ptr), next);
	car_assert_cmp(*get_prev_block(block_ptr), *prev);
	*get_prev_block(block_ptr) = (void *)0x12345678;
	car_assert_cmp(*get_prev_block(block_ptr), (void *)0x12345678);
}

car_test area_access(void)
{
	char area[TOTAL_CAPACITY];
	void *area_ptr = area;

	car_assert_cmp(get_tiny_area(area_ptr), area_ptr);
	car_assert_cmp(get_small_area(area_ptr), (char *)area_ptr + TINY_CAPACITY);
	car_assert_cmp(get_large_area(area_ptr), (char *)area_ptr + TINY_CAPACITY + SMALL_CAPACITY);
}