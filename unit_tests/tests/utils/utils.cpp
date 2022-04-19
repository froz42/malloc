#include "../../car.hpp"
#include <sys/mman.h>

car_test test_get_or_create_area(void)
{
	void *area = get_or_create_area();

	car_assert(area != NULL);
	
	void *area2 = get_or_create_area();

	car_assert_cmp(area, area2);

	car_assert_cmp(munmap(area, TOTAL_CAPACITY), 0);
}