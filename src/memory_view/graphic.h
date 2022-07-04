/**
 * @file graphic.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contain prototypes and defines of graphic library
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "../../minilibx/mlx.h"
#include <stdlib.h>
#include "../malloc.h"

#define WINDOW_X 1300
#define WINDOW_Y 800

#define max(a, b) \
	({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })


/**
 * @brief The frame struct
 * 
 */
typedef struct s_frame
{
	/**
	 * @brief img the mlx image
	 * 
	 */
	void *img;

	/**
	 * @brief addr the mlx image address
	 * 
	 */
	char *addr;

	/**
	 * @brief the number of bits per pixel
	 * 
	 */
	int bits_per_pixel;

	/**
	 * @brief the number of bytes per line
	 * 
	 */
	int line_length;

	/**
	 * @brief the endian
	 * 
	 */
	int endian;

	/**
	 * @brief the x size
	 * 
	 */
	int x;

	/**
	 * @brief the y size
	 * 
	 */
	int y;
} t_frame;


/**
 * @brief The app mode: display blocks or display tree
 * 
 */
typedef enum e_app_mode
{
	/**
	 * @brief display blocks
	 * 
	 */
	DISPLAY_BLOCKS,

	/**
	 * @brief Display tree
	 * 
	 */
	DISPLAY_TREE
} t_app_mode;

/**
 * @brief Area choice: tiny or small
 * 
 */
typedef enum e_app_block
{
	BLOCK_TINY,
	BLOCK_SMALL,
} t_app_block;

/**
 * @brief App state
 * 
 */
typedef struct s_app_state
{
	/**
	 * @brief The mode selected
	 * 
	 */
	t_app_mode mode;

	/**
	 * @brief The tiny or small block selected
	 * 
	 */
	t_app_block block;

	/**
	 * @brief The block selected
	 * 
	 */
	void *block_selected;

	/**
	 * @brief Scroll offset Y
	 * 
	 */
	ssize_t scroll_offset;

	/**
	 * @brief The x where the user clicked
	 * 
	 */
	int x_click;

	/**
	 * @brief The y where the user clicked
	 * 
	 */
	int y_click;
} t_app_state;

/**
 * @brief The mlx structure
 * 
 */
typedef struct s_mlx
{
	/**
	 * @brief The mlx ptr
	 * 
	 */
	void *mlx;

	/**
	 * @brief Window ptr
	 * 
	 */
	void *win;

	/**
	 * @brief The main frame
	 * 
	 */
	t_frame frame;

	/**
	 * @brief App state
	 * 
	 */
	t_app_state state;
} t_mlx;

# include "views.h"

void launch_thread(void);
int loop_hook(t_mlx *mlx);
int mouse_hook(int button, int x, int y, t_mlx *mlx);

void frame_render(t_mlx *mlx);
void post_frame_render(t_mlx *mlx);
void mouse_click_event(int button, int x, int y, t_mlx *mlx);
void init_app_state(t_app_state *app_state);

int rgb_to_color(int r, int g, int b);
void frame_put_pixel(t_frame *f, int x, int y, int color);
void draw_rectangle(t_frame *f, int x, int y, int w, int h, int color);
void draw_line(t_frame *f, int x1, int y1, int x2, int y2, int color);
void put_string(t_mlx *mlx, int x, int y, int color, char *string);
size_t get_text_width(char *string);
int is_in(int x, int y, int w, int h, int x2, int y2);

#endif