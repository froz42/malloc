#include <pthread.h>
#include "../malloc.h"
#include <math.h>
#include "../../minilibx/mlx.h"
#include "memory_view.h"


static void *thread_routine(void *arg)
{
	(void)arg;
	t_mlx	mlx_infos;
	mlx_infos.mlx = mlx_init();
	if (mlx_infos.mlx == NULL)
	{
		log_error("mlx initialization failed, exiting...");
		exit(EXIT_FAILURE);
	}
	mlx_infos.win = mlx_new_window(mlx_infos.mlx, WINDOW_X, WINDOW_Y, "Malloc memory view");
	if (mlx_infos.win == NULL)
	{
		log_error("mlx window creation failed, exiting...");
		exit(EXIT_FAILURE);
	}

}

void launch_thread(void)
{
	pthread_t thread;
	ft_putstr("launch_thread\n");
	pthread_create(&thread, NULL, &thread_routine, NULL);
	ft_putstr("launch thread done\n");
}