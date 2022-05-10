#include "../../car.hpp"
#include <sys/mman.h>
#include <unistd.h>

car_test test_get_or_create_area(void)
{
	void *area = get_or_create_area();

	car_assert(area != NULL);

	// test that area is aligned to page size
	car_assert_cmp((size_t)area % getpagesize(), 0u);

	void *tiny_area = get_tiny_area(area);
	void *small_area = get_small_area(area);

	car_assert_cmp(get_block_size(tiny_area), TINY_CAPACITY - sizeof(void *) * 2);
	car_assert_cmp(get_block_size(small_area), SMALL_CAPACITY - sizeof(void *) * 2);

	// test allignment (16)
	car_assert_cmp((size_t)tiny_area % 16, 0u);
	car_assert_cmp((size_t)small_area % 16, 0u);

	car_assert_cmp(get_next_block(tiny_area), small_area);
	
	void *area2 = get_or_create_area();

	car_assert_cmp(area, area2);



}