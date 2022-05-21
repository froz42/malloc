#include "../../car.hpp"

car_test test_is_allocated(void)
{
	size_t block_header = 0;

	car_assert(!is_allocated(&block_header));
	block_header |= 1;
	car_assert(is_allocated(&block_header));
}

car_test test_set_allocated(void)
{
	size_t block_header = 0;

	set_allocated(&block_header);
	car_assert(is_allocated(&block_header));
}