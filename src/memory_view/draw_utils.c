/**
 * @file draw_utils.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contain the draw utils tools
 * @date 2022-07-04
 *
 */

#include "../malloc.h"
#include <math.h>
#include <stdlib.h>
#include "../../minilibx/mlx.h"
#include "graphic.h"


/**
 * @brief convert rgb to int
 * 
 * @param r red
 * @param g green
 * @param b blue
 * @return int the color
 */
int rgb_to_color(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

/**
 * @brief Put a pixel on a frame and check boundaries
 * 
 * @param f the frame ptr
 * @param x x position
 * @param y y position
 * @param color the color
 */
void frame_put_pixel(t_frame *f, int x, int y, int color)
{
	char *dst;

	if (x <= f->x && y <= f->y && x >= 0 && y >= 0)
	{
		dst = f->addr + (y * f->line_length + x * (f->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

/**
 * @brief Draw a rectangle on a frame
 * 
 * @param f the frame ptr
 * @param x x position
 * @param y y position
 * @param w width
 * @param h height
 * @param color 
 */
void draw_rectangle(t_frame *f, int x, int y, int w, int h, int color)
{
	for (int i = x; i < x + w; i++)
	{
		if (i < 0 || i >= f->x)
			continue;
		for (int j = y; j < y + h; j++)
		{
			if (j < 0 || j >= f->y)
				continue;
			frame_put_pixel(f, i, j, color);
		}
	}
}

/**
 * @brief return sign of a number
 * 
 * @param x the number
 * @return int 0, 1 or -1
 */
static int sign(int x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}

/**
 * @brief return the absolute value of a number
 * 
 * @param x the number
 * @return int the absolute value
 */
static int ft_abs(int x)
{
	if (x < 0)
		return -x;
	return x;
}

/**
 * @brief Draw a line using Bresenham algorithm
 * 
 * @param f the frame ptr
 * @param x1 x position of the first point
 * @param y1 y position of the first point
 * @param x2 x position of the second point
 * @param y2 y position of the second point
 * @param color the color
 */
void draw_line(t_frame *f, int x1, int y1, int x2, int y2, int color)
{
	int x, y, dx, dy, swap, temp, s1, s2, p, i;

	x = x1;
	y = y1;
	dx = ft_abs(x2 - x1);
	dy = ft_abs(y2 - y1);
	s1 = sign(x2 - x1);
	s2 = sign(y2 - y1);
	swap = 0;
	
	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		swap = 1;
	}
	p = 2 * dy - dx;
	for (i = 0; i < dx; i++)
	{
		frame_put_pixel(f, x, y, color);
		while (p >= 0)
		{
			p = p - 2 * dx;
			if (swap)
				x += s1;
			else
				y += s2;
		}
		p = p + 2 * dy;
		if (swap)
			y += s2;
		else
			x += s1;
	}
	frame_put_pixel(f, x2, y2, color);
}


/**
 * @brief Get the text width
 * 
 * @param string the string
 * @return size_t the width
 */
size_t get_text_width(char *string)
{
	size_t width = 0;

	while (*string)
	{
		width += 5;
		string++;
	}
	return (width);
}

/**
 * @brief Check if a point is in a rectangle
 * 
 * @param x x position of rectangle
 * @param y y position of rectangle
 * @param w width of rectangle
 * @param h height of rectangle
 * @param x2 x position of point
 * @param y2 y position of point
 * @return int 1 if point is in rectangle, 0 otherwise
 */
int is_in(int x, int y, int w, int h, int x2, int y2)
{
	return (x2 >= x && x2 < x + w && y2 >= y && y2 < y + h);
}

/**
 * @brief put a string on a the window
 * 
 * @param mlx the mlx ptr
 * @param x x position
 * @param y y position
 * @param color the color
 * @param string the string
 */
void put_string(t_mlx *mlx, int x, int y, int color, char *string)
{
	mlx_string_put(mlx->mlx, mlx->win, x, y, color, string);
}