#include "../../car.hpp"
#include <unistd.h>

car_test define_sizes(void)
{
	const int page_size = getpagesize();

	car_assert_cmp(page_size % TINY_MAX_SIZE, 0);
	car_assert_cmp(page_size % SMALL_MAX_SIZE, 0);

	car_assert_cmp(TINY_CAPACITY, (TINY_MAX_SIZE * 256 + sizeof(size_t) * 256 * 2));
	car_assert_cmp(SMALL_CAPACITY, (SMALL_MAX_SIZE * 256 + sizeof(size_t) * 256 * 2));

	car_assert_cmp(TINY_CAPACITY % page_size, 0u);
	car_assert_cmp(SMALL_CAPACITY % page_size, 0u);

	car_assert_cmp(TOTAL_CAPACITY, (TINY_CAPACITY + SMALL_CAPACITY));
}