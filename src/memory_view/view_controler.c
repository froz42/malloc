#include "graphic.h"
#include "../malloc.h"

void init_app_state(t_app_state *app_state)
{
	app_state->mode = DISPLAY_BLOCKS;
	app_state->scroll_offset = 0;
	app_state->block = BLOCK_TINY;
	app_state->block_selected = (void *)0x0;
}

void button(t_mlx *mlx, int x, int y, int w, int h, int pressed)
{
	// grey rectangle
	draw_rectangle(&mlx->frame, x, y, w, h, 0x888888);
	// white rectangle or dark grey rectangle
	draw_rectangle(&mlx->frame, x + 1, y + 1, w - 2, h - 2, pressed ? 0xFFFFFF : 0xc7c7c7);
}

void area_logic(t_mlx *mlx, void *(callback)(t_mlx *mlx))
{

}

void draw_area(t_mlx *mlx)
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
		unsigned int color = is_allocated(start) ? 0xFF0000 : 0x00FF00;
		ssize_t size_to_draw = size;
		while (size_to_draw > 0)
		{
			if (x >= WINDOW_X - 20)
			{
				x = 20;
				y += 20;
			}
			ssize_t size_to_draw_in_this_line = min(size_to_draw, WINDOW_X - x - 20);
			draw_rectangle(&mlx->frame, x, y, size_to_draw_in_this_line, 10, color);
			x += size_to_draw_in_this_line + 5;
			size_to_draw -= size_to_draw_in_this_line;
		}
		start = get_next_block(start);
	}
}



void block_infos(t_mlx *mlx)
{
	if (!mlx->state.block_selected)
		return ;
	draw_rectangle(&mlx->frame, 20, 20, 150, 250, 0x2e2e2e);
}

void ptr_to_string(char *str, void *ptr)
{
	static char const hex[] = "0123456789ABCDEF";
	size_t size = (size_t)ptr;

	str[0] = '0';
	str[1] = 'x';
	for (int i = sizeof(size_t) * 2 - 1; i >= 2; i--)
	{
		str[i] = hex[size & 0xF];
		size >>= 4;
	}
	str[sizeof(size_t) * 2] = '\0';
}

void size_to_string(char *str, size_t size)
{
	for (int i = sizeof(size_t) * 2 - 1; i >= 0; i--)
	{
		str[i] = size % 10 + '0';
		size /= 10;
	}
	str[sizeof(size_t) * 2] = '\0';
}

void block_infos_text(t_mlx *mlx)
{
	if (!mlx->state.block_selected)
		return ;
	char buff[50];
	put_string(mlx, 160, 35, 0xff0000, "X");
	put_string(mlx, 50, 40, 0xFFFFFF, "Block's infos");
	put_string(mlx, 30, 60, 0xe3e3e3, "Block's address : ");
	// print address of block
	ptr_to_string(buff, mlx->state.block_selected);
	put_string(mlx, 35, 75, 0xFFFFFF, buff);
}

void frame_render(t_mlx *mlx)
{
	draw_area(mlx);
	button(mlx, WINDOW_X - 80, 5, 75, 50, mlx->state.block == BLOCK_TINY);
	button(mlx, WINDOW_X - 80 - 75, 5, 75, 50, mlx->state.block == BLOCK_SMALL);
	block_infos(mlx);
}

void post_frame_render(t_mlx *mlx)
{
	// top right button's text
	put_string(mlx, WINDOW_X - 80 + get_text_width("Small"), 32, 0x0, "Small");
	put_string(mlx, WINDOW_X - 80 - 75 + get_text_width("Tiny"), 32, 0x0, "Tiny");
	block_infos_text(mlx);
}


void key_press_event(int keycode, t_mlx *mlx)
{
	(void)keycode;
	(void)mlx;
}

void key_release_event(int keycode, t_mlx *mlx)
{
	(void)keycode;
	(void)mlx;
}

static inline int is_in(int x, int y, int w, int h, int x2, int y2)
{
	return (x2 >= x && x2 < x + w && y2 >= y && y2 < y + h);
}

void mouse_click_event(int button, int x, int y, t_mlx *mlx)
{
	(void)x;
	(void)y;
	if (button == 4)
		mlx->state.scroll_offset += 20;
	// button 5 scroll down
	else if (button == 5)
		mlx->state.scroll_offset -= 20;
	
	else if (button == 1)
	{
		if (is_in(WINDOW_X - 80, 5, 75, 50, x, y))
			mlx->state.block = BLOCK_SMALL;
		else if (is_in(WINDOW_X - 80 - 75, 5, 75, 50, x, y))
			mlx->state.block = BLOCK_TINY;
	}
}