#include "../car.hpp"

car_test test_ft_malloc(void)
{
	void *start = get_or_create_area();
	
	void *alloc1 = ft_malloc(TINY_MAX_SIZE);
	void *alloc2 = ft_malloc(SMALL_MAX_SIZE);
	void *alloc3 = ft_malloc(SMALL_MAX_SIZE * 2);

	car_assert_cmp(alloc1, get_block_data(get_tiny_area(start)));
	car_assert_cmp(alloc2, get_block_data(get_small_area(start)));
	car_assert_cmp(alloc3, (void *)NULL);

	alloc1 = ft_malloc(TINY_MAX_SIZE);
	alloc2 = ft_malloc(SMALL_MAX_SIZE);

	car_assert_cmp(alloc1, (void *)NULL);
	car_assert_cmp(alloc2, (void *)NULL);
}