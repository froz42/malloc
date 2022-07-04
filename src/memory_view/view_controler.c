/**
 * @file view_controler.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief this file contains the main logic of the view
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "graphic.h"
#include <stdio.h>


/**
 * @brief This function init the app state
 * 
 * @param app_state the app state ptr
 */
void init_app_state(t_app_state *app_state)
{
	app_state->mode = DISPLAY_BLOCKS;
	app_state->scroll_offset = 0;
	app_state->block = BLOCK_TINY;
	app_state->block_selected = (void *)0x0;
	app_state->x_click = -1;
	app_state->y_click = -1;
}

/**
 * @brief Render a button to the frame
 * 
 * @param mlx the mlx structure
 * @param x x position
 * @param y y position
 * @param w width
 * @param h height
 * @param pressed is the button pressed: 0 or 1
 */
void button(t_mlx *mlx, int x, int y, int w, int h, int pressed)
{
	draw_rectangle(&mlx->frame, x, y, w, h, 0x888888);
	draw_rectangle(&mlx->frame, x + 1, y + 1, w - 2, h - 2, pressed ? 0xFFFFFF : 0xc7c7c7);
}

/**
 * @brief Called each loop to draw the frame
 * 
 * @param mlx the mlx structure
 */
void frame_render(t_mlx *mlx)
{
	if (mlx->state.mode == DISPLAY_BLOCKS)
		area_logic(mlx, &render_block);
	else
		tree_logic(mlx, &render_node);
	button(mlx, WINDOW_X - 80, 5, 75, 50, mlx->state.block == BLOCK_TINY);
	button(mlx, WINDOW_X - 80 - 75, 5, 75, 50, mlx->state.block == BLOCK_SMALL);
	button(mlx, WINDOW_X - 80, 65, 75, 50, mlx->state.mode == DISPLAY_BLOCKS);
	button(mlx, WINDOW_X - 80 - 75, 65, 75, 50, mlx->state.mode == DISPLAY_TREE);
	block_infos(mlx);
}

/**
 * @brief Called after rendthe appcture
 */
void post_frame_render(t_mlx *mlx)
{
	put_string(mlx, WINDOW_X - 80 + get_text_width("Small"), 32, 0x0, "Small");
	put_string(mlx, WINDOW_X - 80 - 75 + get_text_width("Tiny"), 32, 0x0, "Tiny");
	put_string(mlx, WINDOW_X - 80 - 90 + get_text_width("Blocks"), 92, 0x0, "Blocks");
	put_string(mlx, WINDOW_X - 80 + get_text_width("Tree"), 92, 0x0, "Tree");
	block_infos_text(mlx);
	if (mlx->state.mode == DISPLAY_TREE)
		tree_logic(mlx, &render_node_text);
}


/**
 * @brief Called each time the window is clicked
 * 
 * @param button the button pressed
 * @param x the x position
 * @param y the y position
 * @param mlx the mlx structure
 */
void mouse_click_event(int button, int x, int y, t_mlx *mlx)
{
	if (button == 4)
		mlx->state.scroll_offset += 20;
	else if (button == 5)
		mlx->state.scroll_offset -= 20;

	else if (button == 1)
	{
		if (is_in(WINDOW_X - 80, 5, 75, 50, x, y))
			mlx->state.block = BLOCK_SMALL;
		else if (is_in(WINDOW_X - 80 - 75, 5, 75, 50, x, y))
			mlx->state.block = BLOCK_TINY;
		else if (is_in(150, 30, 20, 20, x, y))
			mlx->state.block_selected = NULL;
		else if (is_in(WINDOW_X - 80, 65, 75, 50, x, y))
			mlx->state.mode = DISPLAY_TREE;
		else if (is_in(WINDOW_X - 80 - 75, 65, 75, 50, x, y))
			mlx->state.mode = DISPLAY_BLOCKS;
		else
		{
			mlx->state.x_click = x;
			mlx->state.y_click = y;
			if (mlx->state.mode == DISPLAY_BLOCKS)
				area_logic(mlx, &click_block);
			else 
				tree_logic(mlx, &click_node);
			mlx->state.x_click = -1;
			mlx->state.y_click = -1;
		}
	}
}