/**
 * @file area_view.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contain the area view functions
 * @date 2022-07-04
 * 
 */

#include "graphic.h"

/**
 * @brief this function is used to draw a block of memory
 * 
 * @param mlx the mlx structure
 * @param x the x position of the block
 * @param y the y position of the block
 * @param size_to_draw_in_this_line the size of the block
 * @param block the block to draw
 */
void render_block(t_mlx *mlx,
				int x,
				int y,
				int size_to_draw_in_this_line,
				block_ptr block)
{
	if (mlx->state.block_selected == block)
		draw_rectangle(&mlx->frame, x - 1, y - 1, size_to_draw_in_this_line + 2, 10 + 2, 0xFFFFFF);
	unsigned int color = is_allocated(block) ? 0xFF0000 : 0x00FF00;
	draw_rectangle(&mlx->frame, x, y, size_to_draw_in_this_line, 10, color);
}

/**
 * @brief this function is used to detect if the user clicked on a block of memory
 * 
 * @param mlx the mlx structure
 * @param x the x position of the block
 * @param y the y position of the block
 * @param size_to_draw_in_this_line the size of the block
 * @param block the block to select if the user clicked on it
 */
void click_block(t_mlx *mlx,
				 int x,
				 int y,
				 int size_to_draw_in_this_line,
				 block_ptr block)
{
	if (mlx->state.x_click >= x && mlx->state.x_click < x + size_to_draw_in_this_line &&
		mlx->state.y_click >= y && mlx->state.y_click < y + 10)
	{
		mlx->state.block_selected = block;
	}
}

/**
 * @brief this function is used as a logic function to draw / detect a block of memory
 * 
 * @param mlx the mlx structure
 * @param callback the callback function to call on each block
 */
void area_logic(
	t_mlx *mlx,
	void(callback)(t_mlx *mlx,
				   int x,
				   int y,
				   int size_to_draw_in_this_line,
				   block_ptr block))
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
		ssize_t size_to_draw = size;
		while (size_to_draw > 0)
		{
			if (x >= WINDOW_X - 20)
			{
				x = 20;
				y += 20;
			}
			ssize_t size_to_draw_in_this_line = min(size_to_draw, WINDOW_X - x - 20);
			callback(mlx, x, y, size_to_draw_in_this_line, start);
			x += size_to_draw_in_this_line + 5;
			size_to_draw -= size_to_draw_in_this_line;
		}
		start = get_next_block(start);
	}
}