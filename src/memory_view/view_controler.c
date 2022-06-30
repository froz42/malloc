#include "graphic.h"
#include "../malloc.h"

void init_app_state(t_app_state *app_state)
{
	app_state->mode = DISPLAY_BLOCKS;
	app_state->scroll_offset = 0;
	app_state->block = BLOCK_TINY;
}

void button(t_mlx *mlx, int x, int y, int w, int h, int pressed)
{
	// grey rectangle
	draw_rectangle(&mlx->frame, x, y, w, h, 0x888888);
	// white rectangle or dark grey rectangle
	draw_rectangle(&mlx->frame, x + 1, y + 1, w - 2, h - 2, pressed ? 0xFFFFFF : 0xc7c7c7);
}

void draw_area(t_mlx *mlx)
{
	area_ptr const area = get_or_create_area();
	if (area == NULL)
		return;
	block_ptr start;
	block_ptr end;
	double size_divider;

	if (mlx->state.block == BLOCK_TINY)
	{
		start = area;
		end = get_small_area(area);
		size_divider = 1.5;
	}
	else
	{
		start = get_small_area(area);
		end = get_large_area(area);
		size_divider = 25;
	}

	ssize_t x = 20;
	ssize_t y = 60 + mlx->state.scroll_offset;

	while (start < end)
	{
		ssize_t size = get_block_size(start) / size_divider;
		unsigned int color = is_allocated(start) ? 0xFF0000 : 0x00FF00;
		ssize_t size_to_draw = size;
		while (size_to_draw > 0)
		{
			if (x >= WINDOW_X - 20)
			{
				x = 20;
				y += 20;
			}
			ssize_t size_to_draw_in_this_line = min(size_to_draw, WINDOW_X - x - 20);
			draw_rectangle(&mlx->frame, x, y, size_to_draw_in_this_line, 10, color);
			x += size_to_draw_in_this_line + 5;
			size_to_draw -= size_to_draw_in_this_line;
		}
		start = get_next_block(start);
	}
}

void frame_render(t_mlx *mlx)
{
	draw_area(mlx);
	button(mlx, WINDOW_X - 80, 5, 75, 50, mlx->state.block == BLOCK_SMALL);
	button(mlx, WINDOW_X - 80 - 75, 5, 75, 50, mlx->state.block == BLOCK_TINY);
	/*block_ptr area = get_or_create_area();
	if (area == NULL)
		return;
	area_ptr tiny_end = get_small_area(area);
	ssize_t x = 20;
	ssize_t y = 300 + mlx->state.scroll_offset;

	while (area < tiny_end)
	{
		ssize_t size = get_block_size(area) / 1.5;
		unsigned int color = is_allocated(area) ? 0xFF0000 : 0x00FF00;
		ssize_t size_to_draw = size;
		while (size_to_draw)
		{
			if (x >= WINDOW_X - 20)
			{
				x = 20;
				y += 20;
			}
			ssize_t size_to_draw_in_this_line = min(size_to_draw, WINDOW_X - x - 20);
			draw_rectangle(&mlx->frame, x, y, size_to_draw_in_this_line, 10, color);
			x += size_to_draw_in_this_line + 5;
			size_to_draw -= size_to_draw_in_this_line;
		}
		area = get_next_block(area);
	}*/
}

void post_frame_render(t_mlx *mlx)
{
	// top right button's text
	put_string(mlx, WINDOW_X - 80 + get_text_width("Tiny"), 32, 0x0, "Tiny");
	put_string(mlx, WINDOW_X - 80 - 75 + get_text_width("Small"), 32, 0x0, "Small");
}


void key_press_event(int keycode, t_mlx *mlx)
{
	(void)keycode;
	(void)mlx;
}

void key_release_event(int keycode, t_mlx *mlx)
{
	(void)keycode;
	(void)mlx;
}

static inline int is_in(int x, int y, int w, int h, int x2, int y2)
{
	return (x2 >= x && x2 < x + w && y2 >= y && y2 < y + h);
}

void mouse_click_event(int button, int x, int y, t_mlx *mlx)
{
	(void)x;
	(void)y;
	if (button == 4)
		mlx->state.scroll_offset += 20;
	// button 5 scroll down
	else if (button == 5)
		mlx->state.scroll_offset -= 20;
	
	else if (button == 1)
	{
		if (is_in(WINDOW_X - 80, 5, 75, 50, x, y))
			mlx->state.block = BLOCK_TINY;
		else if (is_in(WINDOW_X - 80 - 75, 5, 75, 50, x, y))
			mlx->state.block = BLOCK_SMALL;
	}
}