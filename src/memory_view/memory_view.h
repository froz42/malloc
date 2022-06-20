#ifndef MEMORY_VIEW_H
#define MEMORY_VIEW_H

# define WINDOW_X 900
# define WINDOW_Y 600

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

#endif