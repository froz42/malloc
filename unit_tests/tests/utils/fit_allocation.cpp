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

void find_fit_test(void)
{
	void *area = get_or_create_area();

	void *fit_tiny = find_fit(area, TINY_MAX_SIZE);
	void *fit_small = find_fit(area, SMALL_MAX_SIZE);
	void *fit_large = find_fit(area, SMALL_MAX_SIZE * 2);

	car_assert_cmp(fit_tiny, get_tiny_area(area));
	car_assert_cmp(fit_small, get_small_area(area));
	car_assert_cmp(fit_large, (void *)NULL);
}