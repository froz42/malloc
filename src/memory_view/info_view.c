/**
 * @file info_view.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contain the info view functions
 * @date 2022-07-04
 *
 */

#ifdef BONUS

#include "graphic.h"

/**
 * @brief
 *
 * @param mlx
 */
void block_infos(t_mlx *mlx)
{
	if (!mlx->state.block_selected)
		return;
	draw_rectangle(&mlx->frame, 20, 20, 150, 250, 0x2e2e2e);
}

void block_infos_text(t_mlx *mlx)
{
	if (!mlx->state.block_selected)
		return;
	char buff[50];
	put_string(mlx, 160, 35, 0xff0000, "X");
	put_string(mlx, 50, 40, 0xFFFFFF, "Block's infos");
	put_string(mlx, 30, 60, 0xe3e3e3, "Block's address : ");
	ptr_to_string(buff, mlx->state.block_selected);
	put_string(mlx, 35, 75, 0xFFFFFF, buff);
	put_string(mlx, 30, 90, 0xe3e3e3, "Block's size : ");
	size_to_string(buff, get_block_size(mlx->state.block_selected));
	put_string(mlx, 35, 105, 0xFFFFFF, buff);
	put_string(mlx, 30, 120, 0xe3e3e3, "Block's previous : ");
	ptr_to_string(buff, *get_prev_block(mlx->state.block_selected));
	put_string(mlx, 35, 135, 0xFFFFFF, buff);
	put_string(mlx, 30, 150, 0xe3e3e3, "Block's next : ");
	ptr_to_string(buff, get_next_block(mlx->state.block_selected));
	put_string(mlx, 35, 165, 0xFFFFFF, buff);
	put_string(mlx, 30, 180, 0xe3e3e3, "Block allocated : ");
	int allocated = is_allocated(mlx->state.block_selected);
	put_string(mlx, 35, 195, !allocated ? 0x00FF00 : 0xFF0000, allocated ? "true" : "false");
	put_string(mlx, 30, 210, 0xe3e3e3, "Block's data ptr: ");
	ptr_to_string(buff, get_block_data(mlx->state.block_selected));
	put_string(mlx, 35, 225, 0xFFFFFF, buff);
}

#endif
