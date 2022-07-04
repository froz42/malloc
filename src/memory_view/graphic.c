#include <pthread.h>
#include <math.h>
#include "graphic.h"
#include <errno.h>
#include <string.h>

static t_frame	frame_init(void *mlx, int x, int y)
{
	t_frame	frame;

	frame.img = mlx_new_image(mlx, x, y);
	if (!frame.img)
	{
		ft_putstr(strerror(errno));
		return (frame);
	}
	frame.addr = mlx_get_data_addr(frame.img, &frame.bits_per_pixel,
			&frame.line_length, &frame.endian);
	if (!frame.addr)
	{
		ft_putstr(strerror(errno));
		return (frame);
	}
	frame.x = x;
	frame.y = y;
	return (frame);
}

static void *thread_routine(void *arg)
{
	(void)arg;
	t_mlx	mlx_infos;
	
	mlx_infos.mlx = mlx_init();
	if (mlx_infos.mlx == NULL)
	{
		error_write("mlx initialization failed, exiting...");
		return (NULL);
	}
	
	mlx_infos.win = mlx_new_window(mlx_infos.mlx, WINDOW_X, WINDOW_Y, "Malloc memory view");
	if (mlx_infos.win == NULL)
	{
		error_write("mlx window creation failed");
		mlx_destroy_display(mlx_infos.mlx);
		free(mlx_infos.mlx);
		return (NULL);
	}
	
	mlx_infos.frame = frame_init(mlx_infos.mlx, WINDOW_X, WINDOW_Y);
	if (!mlx_infos.frame.img || !mlx_infos.frame.addr)
	{
		error_write("mlx frame creation failed");
		mlx_destroy_window(mlx_infos.mlx, mlx_infos.win);
		mlx_destroy_display(mlx_infos.mlx);
		free(mlx_infos.mlx);
		return (NULL);
	}

	init_app_state(&mlx_infos.state);
	
	mlx_mouse_hook(mlx_infos.win, mouse_hook, &mlx_infos);
	mlx_loop_hook(mlx_infos.mlx, loop_hook, &mlx_infos);
	mlx_loop(mlx_infos.mlx);

	return (NULL);
}

void launch_thread(void)
{
	pthread_t thread;
	ft_putstr("launch_thread\n");
	pthread_create(&thread, NULL, &thread_routine, NULL);
	ft_putstr("launch thread done\n");
}