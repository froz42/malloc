#include "../../car.hpp"

car_test test_parse_config(void)
{
	t_config config = parse_config("verbose");

	car_assert(config.verbose);

	config = parse_config("yolo");

	car_assert(!config.verbose);

	config = parse_config("verbos, yolo");

	car_assert(!config.verbose);
	

}