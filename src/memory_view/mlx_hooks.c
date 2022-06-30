#include "graphic.h"
#include <stdio.h>
#include <unistd.h>
#include "../malloc.h"

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

int key_press_hook(int keycode, t_mlx *mlx)
{
	if (SHOW_KEY_PRESSED)
		printf("Key pressed code: %d\n", keycode);
	key_press_event(keycode, mlx);
	return (0);
}

int key_release_hook(int keycode, t_mlx *mlx)
{
	if (SHOW_KEY_PRESSED)
		printf("Key released code: %d\n", keycode);
	key_release_event(keycode, mlx);
	return (0);
}

int mouse_hook(int button, int x, int y, t_mlx *mlx)
{
	if (SHOW_KEY_PRESSED)
		printf("Mouse clicked: button: %d, x: %d, y: %d\n", button, x, y);
	mouse_click_event(button, x, y, mlx);
	return (0);
}