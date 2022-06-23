#include "../malloc.h"
#include <math.h>
#include <stdlib.h>
#include "../../minilibx/mlx.h"
#include "graphic.h"


int rgb_to_color(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

void frame_put_pixel(t_frame *f, int x, int y, int color)
{
	char *dst;

	if (x <= f->x && y <= f->y && x >= 0 && y >= 0)
	{
		dst = f->addr + (y * f->line_length + x * (f->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

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

static int sign(int x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}

void draw_line(t_frame *f, int x1, int y1, int x2, int y2, int color)
{
	int x, y, dx, dy, swap, temp, s1, s2, p, i;

	x = x1;
	y = y1;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
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

void put_string(t_mlx *mlx, int x, int y, int color, char *string)
{
	mlx_string_put(mlx->mlx, mlx->win, x, y, color, string);
}