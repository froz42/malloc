#include "../../car.hpp"
#include <unistd.h>

car_test define_sizes(void)
{
	const int page_size = getpagesize();

	car_assert_cmp(page_size % TINY_MAX_SIZE, 0);
	car_assert_cmp(page_size % SMALL_MAX_SIZE, 0);

	car_assert_cmp(TINY_CAPACITY, (TINY_MAX_SIZE * 256 + sizeof(size_t) * 256 * 4));
	car_assert_cmp(SMALL_CAPACITY, (SMALL_MAX_SIZE * 256 + sizeof(size_t) * 256 * 4));

	car_assert_cmp(TINY_CAPACITY % page_size, 0u);
	car_assert_cmp(SMALL_CAPACITY % page_size, 0u);

	car_assert_cmp(TOTAL_CAPACITY, (TINY_CAPACITY + SMALL_CAPACITY));
	// test MINIMAL_SIZE
	car_assert_cmp(MINIMAL_SIZE, 48u);
	car_assert_cmp(MINIMAL_SIZE, ALLIGN_16(MINIMAL_SIZE));
}

car_test define_utils(void)
{
	// test ALLIGN_16(x)
	car_assert_cmp(ALLIGN_16(0), 0);
	car_assert_cmp(ALLIGN_16(1), 16);
	car_assert_cmp(ALLIGN_16(2), 16);
	car_assert_cmp(ALLIGN_16(40), 48);
	car_assert_cmp(ALLIGN_16(41), 48);
	car_assert_cmp(ALLIGN_16(16), 16);
	car_assert_cmp(ALLIGN_16(17), 32);
}