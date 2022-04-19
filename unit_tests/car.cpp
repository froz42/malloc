#include "car.hpp"

int success_assert = 0;
int fail_assert = 0;
std::string filter = "";
bool has_module_failled_yet = false;
std::string module_name = "";

#ifdef __linux__
#define OS "Linux"
#elif __APPLE__
#define OS "MacOS"
#else
#define OS "Unknow"
#endif

void show_module_head(std::string module_name, bool ok)
{
	if (filter != "")
		return;
	std::cout << "Module " << BOLD << module_name << " " << RESET;
	if (ok)
		std::cout << GREEN << "[PASS]" << RESET << std::endl;
	else
		std::cout << RED << "[FAIL]" << RESET << std::endl;
}

static void session_end(int success_module, int module_count)
{
	std::cout << std::endl;
	if (!fail_assert)
		std::cout << GREEN
				  << "========================== TEST SESSION END ==========================="
				  << RESET << std::endl;
	else
		std::cout << BOLD << RED
				  << "========================== TEST SESSION END ==========================="
				  << RESET << std::endl;
	if (!(success_assert + fail_assert))
		std::cout << "No test has been run" << std::endl;
	else
		std::cout << "Summary: " << std::endl
				  << "modules: " << BOLD << success_module << "/" << module_count << RESET
				  << " (" << ((success_module * 100) / module_count) << "%)" << std::endl
				  << "asserts: " << BOLD << success_assert << "/" << success_assert + fail_assert << RESET
				  << " (" << ((success_assert * 100) / (success_assert + fail_assert)) << "%)"
				  << RESET << std::endl;
}

int run_tests(test_function *tests[])
{
	success_assert = 0;
	fail_assert = 0;
	int success_module = 0;
	int module_count = 0;

	int nb_tests;
	for (nb_tests = 0; tests[nb_tests] != NULL; nb_tests++)
		;
	std::cout << BOLD
			  << "========================== TEST SESSION START =========================="
			  << RESET << std::endl;
	std::cout << "Build date: " << __DATE__ << " " << __TIME__ << std::endl;
	std::cout << "OS: " << OS << std::endl;
	std::cout << "Compiler: " << __VERSION__ << std::endl
			  << std::endl;
	std::cout << BOLD << "Functions collected: " << nb_tests << RESET << std::endl;
	if (filter != "")
		std::cout << BOLD << "Filter: '" << filter << "'" << RESET << std::endl;
	std::cout << std::endl;

	for (int i = 0; tests[i] != NULL; i++)
	{
		module_count++;
		has_module_failled_yet = false;
		tests[i]();
		if (!has_module_failled_yet)
		{
			show_module_head(module_name, true);
			success_module++;
		}
	}

	session_end(success_module, module_count);
	return (fail_assert != 0);
}
