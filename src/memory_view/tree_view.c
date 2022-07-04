/**
 * @file tree_view.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief This file contain the tree view functions
 * @date 2022-07-04
 * 
 */

#include "graphic.h"
#include "../malloc.h"


/**
 * @brief This function is called to render the text of a node
 * 
 * @param mlx the mlx structure
 * @param block the node
 * @param x x where the node is drawn
 * @param y y where the node is drawn
 */
void render_node_text(t_mlx *mlx, block_ptr block, int x, int y)
{
	char buff[50];

	put_string(mlx, x + 5, y + 10, 0x0, "block ptr:");
	ptr_to_string(buff, block);
	put_string(mlx, x + 5, y + 25, 0x0, buff);
	put_string(mlx, x + 5, y + 40, 0x0, "size:");
	size_to_string(buff, get_block_size(block));
	put_string(mlx, x + 5, y + 55, 0x0, buff);
	put_string(mlx, x + 5, y + 70, 0x0, "color:");
	put_string(mlx, x + 50, y + 70,
			   *get_color(block) == RED ? 0xFF0000 : 0x0,
			   *get_color(block) == RED ? "red" : "black");
	if (get_nil_node() == block)
		put_string(mlx, x + 85, y + 70, 0x0, "(nil)");
}

/**
 * @brief Render a node of the tree
 * 
 * @param mlx the mlx structure
 * @param block the node
 * @param x x where the node is drawn
 * @param y y where the node is drawn
 */
void render_node(t_mlx *mlx, block_ptr block, int x, int y)
{
	draw_rectangle(&mlx->frame, x - 5, y - 5, 125 + 10, 75 + 10,
				   block == mlx->state.block_selected ? 0x3464eb : 0x2e2e2e);
	draw_rectangle(&mlx->frame, x, y, 125, 75, 0xFFFFFF);
	if (*get_left_child(block) != get_nil_node())
		draw_line(&mlx->frame, x + 62.5, y + 75, x - 80 + 62.5, y + 90, 0xFFFFFF);
	if (*get_right_child(block) != get_nil_node())
		draw_line(&mlx->frame, x + 62.5, y + 75, x + 80 + 62.5, y + 90, 0xFFFFFF);
}

/**
 * @brief This function is called to render to determine if the user clicked on a node
 * 
 * @param mlx the mlx structure
 * @param block the node to select if the user clicked on it
 * @param x x where the node is drawn
 * @param y y where the node is drawn
 */
void click_node(t_mlx *mlx, block_ptr block, int x, int y)
{
	if (mlx->state.x_click >= x && mlx->state.x_click <= x + 125 &&
		mlx->state.y_click >= y && mlx->state.y_click <= y + 75)
	{
		mlx->state.block_selected = block;
	}
}

/**
 * @brief Render a tree recursively
 * 
 * @param mlx the mlx structure
 * @param root the node of the tree
 * @param x x where the tree is drawn
 * @param y y where the tree is drawn
 * @param callback the callback function to draw / click on a node
 */
static void render_tree_recursive(
	t_mlx *mlx,
	block_ptr *root,
	int x,
	int y,
	void(callback)(
		t_mlx *mlx,
		block_ptr block,
		int x,
		int y))
{
	if (*get_left_child(*root) != get_nil_node())
		render_tree_recursive(mlx, get_left_child(*root), x - 80, y + 90, callback);
	if (*get_right_child(*root) != get_nil_node())
		render_tree_recursive(mlx, get_right_child(*root), x + 80, y + 90, callback);
	callback(mlx, *root, x, y);
}

/**
 * @brief Draw the tree
 * 
 * @param mlx the mlx structure
 * @param callback the callback function to draw / click on a node
 */
void tree_logic(t_mlx *mlx, void(callback)(t_mlx *mlx, block_ptr block, int x, int y))
{
	free_tree_t *tree = get_free_trees();
	block_ptr *root = mlx->state.block == BLOCK_TINY
						  ? &tree->tiny
						  : &tree->small;
	render_tree_recursive(mlx, root, WINDOW_X / 2 - 62.5, 30 + mlx->state.scroll_offset, callback);
}
