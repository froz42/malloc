/**
 * @file mlx_hooks.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief Contains hooks functions
 * @date 2022-07-04
 * 
 */

#ifdef BONUS

#include "graphic.h"
#include <stdio.h>
#include <unistd.h>
#include "../malloc.h"

/**
 * @brief This function is called every loop to draw the frame
 * 
 * @param mlx the mlx structure
 * @return int 0
 */
int loop_hook(t_mlx *mlx)
{
	pthread_mutex_lock(&g_malloc_mutex);
	draw_rectangle(&mlx->frame, 0, 0, WINDOW_X, WINDOW_Y, 0);
	frame_render(mlx);
	mlx_put_image_to_window(
		mlx->mlx,
		mlx->win,
		mlx->frame.img,
		0,
		0);
	post_frame_render(mlx);
	pthread_mutex_unlock(&g_malloc_mutex);
	usleep(10000);
	return (0);
}

/**
 * @brief This function is called when the user click on the window
 * 
 * @param button the button pressed
 * @param x x where the user clicked
 * @param y y where the user clicked
 * @param mlx the mlx structure
 * @return int 0
 */
int mouse_hook(int button, int x, int y, t_mlx *mlx)
{
	mouse_click_event(button, x, y, mlx);
	return (0);
}

#endif