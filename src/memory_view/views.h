/**
 * @file views.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contain prototypes and defines of graphic library
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef VIEW_H
# define VIEW_H

# include "graphic.h"

void tree_logic(t_mlx *mlx, void (callback)(t_mlx *mlx, block_ptr block, int x, int y));
void render_node(t_mlx *mlx, block_ptr block, int x, int y);
void render_node_text(t_mlx *mlx, block_ptr block, int x, int y);
void click_node(t_mlx *mlx, block_ptr block, int x, int y);

void render_block(t_mlx *mlx,
				int x,
				int y,
				int size_to_draw_in_this_line,
				block_ptr block);

void click_block(t_mlx *mlx,
				 int x,
				 int y,
				 int size_to_draw_in_this_line,
				 block_ptr block);

void area_logic(
	t_mlx *mlx,
	void(callback)(t_mlx *mlx,
				   int x,
				   int y,
				   int size_to_draw_in_this_line,
				   block_ptr block));

void block_infos(t_mlx *mlx);
void block_infos_text(t_mlx *mlx);

#endif