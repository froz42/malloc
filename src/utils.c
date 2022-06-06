/**
 * @file utils.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contains various utilities
 * @date 2022-06-06
 * 
 */

#include <unistd.h>

/**
 * @brief write error message to stderr
 * 
 * @param str error message
 */
void error_write(char *str)
{
	size_t len = 0;
	while (str[len] != '\0')
		len++;
	write(STDERR_FILENO, str, len);
	write(STDERR_FILENO, "\n", 1);
}

void ft_putstr(char *str)
{
	size_t i = 0;
	while (str[i])
		i++;
	write(1, str, i);
}

void ft_putnbr(size_t n)
{
	if (n >= 10)
		ft_putnbr(n / 10);
	char c;
	c = n % 10 + '0';
	write(1, &c, 1);
}