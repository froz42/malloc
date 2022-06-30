#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "../../minilibx/mlx.h"
#include <stdlib.h>

#define WINDOW_X 1000
#define WINDOW_Y 800
#define SHOW_KEY_PRESSED 0

#define max(a, b) \
	({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })

typedef struct s_frame
{
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
	int x;
	int y;
} t_frame;

typedef enum e_app_mode
{
	DISPLAY_BLOCKS,
} t_app_mode;

typedef enum e_app_block
{
	BLOCK_TINY,
	BLOCK_SMALL,
} t_app_block;

typedef struct s_app_state
{
	t_app_mode mode;
	t_app_block block;
	void *block_selected;
	ssize_t scroll_offset;
	int x_click;
	int y_click;
} t_app_state;

typedef struct s_mlx
{
	void *mlx;
	void *win;
	t_frame frame;
	t_app_state state;
} t_mlx;

void launch_thread(void);
int loop_hook(t_mlx *mlx);
int key_press_hook(int keycode, t_mlx *mlx);
int key_release_hook(int keycode, t_mlx *mlx);
int mouse_hook(int button, int x, int y, t_mlx *mlx);

void frame_render(t_mlx *mlx);
void post_frame_render(t_mlx *mlx);
void key_press_event(int keycode, t_mlx *mlx);
void key_release_event(int keycode, t_mlx *mlx);
void mouse_click_event(int button, int x, int y, t_mlx *mlx);
void init_app_state(t_app_state *app_state);

int rgb_to_color(int r, int g, int b);
void frame_put_pixel(t_frame *f, int x, int y, int color);
void draw_rectangle(t_frame *f, int x, int y, int w, int h, int color);
void draw_line(t_frame *f, int x1, int y1, int x2, int y2, int color);
void put_string(t_mlx *mlx, int x, int y, int color, char *string);
size_t get_text_width(char *string);

#endif