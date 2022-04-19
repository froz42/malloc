#ifndef CAR_HPP
#define CAR_HPP

#include <iostream>
#include <iomanip>

extern "C"
{
#include "../src/malloc.h"
}

#include "../src/malloc.h"

#define car_assert(x) car_assert_fn(x, __LINE__, __FILE__, __func__, #x)
#define car_assert_cmp(actual, expected) car_assert_cmp_fn(actual, expected, __LINE__, __FILE__, __func__, #actual, #expected)

typedef void car_test;

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define RESET "\033[0m"

typedef void test_function(void);

int run_tests(test_function *tests[]);
void show_module_head(std::string module_name, bool ok);

extern int success_assert;
extern int fail_assert;
extern std::string filter;
extern bool has_module_failled_yet;
extern std::string module_name;

template <typename T>
void car_assert_fn(T x, int line, std::string filename, std::string funcname,
				   std::string x_str)
{
	module_name = funcname;
	if (!x)
	{
		if (!has_module_failled_yet)
		{
			show_module_head(module_name, false);
			has_module_failled_yet = true;
		}
		std::cout << CYAN << BOLD << " >>> " << RESET
				  << RED << "[✖] " << BLUE << "Assertion"
				  << RED << " failed: " << RESET << BOLD << x_str << RESET << " in " << funcname
				  << "(" << filename << ":" << line << ")" << std::endl;
		fail_assert++;
	}
	else
	{
		success_assert++;
		if (filter != "" && (filter == "--show-all" || funcname.find(filter) != std::string::npos))
			std::cout << CYAN << BOLD << " >>> " << RESET
					  << GREEN << "[✔] " << BLUE << "Assertion"
					  << GREEN << " passed: " << RESET << BOLD << x_str << RESET << " in " << funcname
					  << "(" << filename << ":" << line << ")" << std::endl;
	}
}

template <typename T1, typename T2>
void car_assert_cmp_fn(T1 x, T2 y, int line, std::string filename, std::string funcname,
					   std::string x_str, std::string y_str)
{
	module_name = funcname;
	if (x != y)
	{
		if (!has_module_failled_yet)
		{
			show_module_head(module_name, false);
			has_module_failled_yet = true;
		}
		std::cout << CYAN << BOLD << " >>> " << RESET
				  << RED << "[✖] " << BLUE << "Assertion"
				  << RED << " failed: " << RESET << BOLD << x_str << RESET << " != " << BOLD << y_str << RESET << " in " << funcname
				  << "(" << filename << ":" << line << ")" << std::endl;
		std::cout << std::setw(27) << "(" << RED << x
				  << RESET << " != " << GREEN
				  << y << RESET << ")" << std::endl;
		fail_assert++;
	}
	else
	{
		success_assert++;
		if (filter != "" && (filter == "--show-all" || funcname.find(filter) != std::string::npos))
			std::cout << CYAN << BOLD << " >>> " << RESET
					  << GREEN << "[✔] " << BLUE << "Assertion"
					  << GREEN << " passed: " << RESET << BOLD << x_str << RESET << " == " << BOLD << y_str << RESET << " in " << funcname
					  << "(" << filename << ":" << line << ")" << std::endl;
	}
}

#endif
