#include "../../car.hpp"

car_test find_fit_test(void)
{
	void *area = get_or_create_area();

	void *fit_tiny = find_fit(area, TINY_MAX_SIZE);
	void *fit_small = find_fit(area, SMALL_MAX_SIZE);
	void *fit_large = find_fit(area, SMALL_MAX_SIZE * 2);

	car_assert_cmp(fit_tiny, get_tiny_area(area));
	car_assert_cmp(fit_small, get_small_area(area));
	car_assert_cmp(fit_large, (void *)NULL);
}