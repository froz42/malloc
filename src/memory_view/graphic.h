#ifndef GRAPHIC_H
#define GRAPHIC_H

# include "../../minilibx/mlx.h"

# define WINDOW_X 900
# define WINDOW_Y 600
# define SHOW_KEY_PRESSED 1

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

typedef struct s_frame
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		x;
	int		y;
} t_frame;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	t_frame	frame;
} t_mlx;

void launch_thread(void);
int	loop_hook(t_mlx *mlx);
int key_press_hook(int keycode, t_mlx *mlx);
int key_release_hook(int keycode, t_mlx *mlx);
int mouse_hook(int button, int x, int y, t_mlx *mlx);

void frame_render(t_mlx *mlx);
void post_frame_render(t_mlx *mlx);
void key_press_event(int keycode, t_mlx *mlx);
void key_release_event(int keycode, t_mlx *mlx);
void mouse_click_event(int button, int x, int y, t_mlx *mlx);

#endif