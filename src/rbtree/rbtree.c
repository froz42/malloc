/**
 * @file rbtree.c
 * @author tmatis (tmatis@student.42.fr)
 * @date 2022-06-06
 * 
 * @brief This file contains the implementation of the rbtree.
 * The rbtree is a red-black tree.
 * It is used to store free blocks and to find the best fit block.
 * The rbtree is implemented as a binary tree.
 * The nil node points to itself.
 * the block structure is as follows:
 * ----------------------------------------------------------
 * | size | prev | left | right | parent | padding |  color |
 * ----------------------------------------------------------
32er RED or BLACK.
 */

#include <stdlib.h>
#include "rbtree.h"
#include <stdio.h>


/**
 * @brief Get the nil node object
 * parent, left and right -> nil
 * @return block_ptr 
 */
block_ptr get_nil_node(void)
{
	static char nil_node[MINIMAL_SIZE + sizeof(void *) + sizeof(size_t)];
	static int is_init = 0;

	if (is_init == 0)
	{
		init_area(nil_node, MINIMAL_SIZE + sizeof(void *) + sizeof(size_t));
		*get_parent(nil_node) = nil_node;
		*get_left_child(nil_node) = nil_node;
		*get_right_child(nil_node) = nil_node;
		*get_color(nil_node) = BLACK;
		is_init = 1;
	}
	return (nil_node);
}
/**
 * @brief Get the free trees object
 * 
 * @return free_tree_t* 
 */
free_tree_t *get_free_trees(void)
{
	static free_tree_t free_trees = {NULL, NULL};

	if (free_trees.tiny == NULL)
	{
		free_trees.tiny = get_nil_node();
		free_trees.small = get_nil_node();
	}
	return (&free_trees);
}

/**
 * @brief transplant the node u with node v
 * 
 * @param u the node to be transplant
 * @param v the node to be transplanted
 * @param root the root of the tree
 */
void transplant(block_ptr u, block_ptr v, block_ptr *root)
{
	if (*get_parent(u) == get_nil_node())
		*root = v;
	else if (u == *get_left_child(*get_parent(u)))
		*get_left_child(*get_parent(u)) = v;
	else
		*get_right_child(*get_parent(u)) = v;
	if (*get_parent(v) != get_nil_node())
		*get_parent(v) = *get_parent(u);
}


/**
 * @brief rotate the tree to the left
 * 
 * @param x the node to be rotated
 * @param root the root of the tree
 */
void rotate_left(block_ptr x, block_ptr *root)
{
	block_ptr const y = *get_right_child(x);

	*get_right_child(x) = *get_left_child(y);
	if (*get_left_child(y) != get_nil_node())
		*get_parent(*get_left_child(y)) = x;
	*get_parent(y) = *get_parent(x);
	if (*get_parent(x) == get_nil_node())
		*root = y;
	else if (x == *get_left_child(*get_parent(x)))
		*get_left_child(*get_parent(x)) = y;
	else
		*get_right_child(*get_parent(x)) = y;
	*get_left_child(y) = x;
	*get_parent(x) = y;
}


/**
 * @brief rotate the tree to the right
 * 
 * @param x the node to be rotated
 * @param root the root of the tree
 */
void rotate_right(block_ptr x, block_ptr *root)
{
	block_ptr const y = *get_left_child(x);

	*get_left_child(x) = *get_right_child(y);
	if (*get_right_child(y) != get_nil_node())
		*get_parent(*get_right_child(y)) = x;
	*get_parent(y) = *get_parent(x);
	if (*get_parent(x) == get_nil_node())
		*root = y;
	else if (x == *get_right_child(*get_parent(x)))
		*get_right_child(*get_parent(x)) = y;
	else
		*get_left_child(*get_parent(x)) = y;
	*get_right_child(y) = x;
	*get_parent(x) = y;
}


/**
 * @brief insert a node to the tree
 * 
 * @param root the root of the tree
 * @param n the node to be inserted
 */
void insert_recursive(block_ptr *root, block_ptr n)
{
	if (*root != get_nil_node() && get_block_size(*root) > get_block_size(n))
	{
		if (*get_left_child(*root) != get_nil_node())
			return insert_recursive(get_left_child(*root), n);
		else
			*get_left_child(*root) = n;
	}
	else if (*root != get_nil_node())
	{
		if (*get_right_child(*root) != get_nil_node())
			return insert_recursive(get_right_child(*root), n);
		else
			*get_right_child(*root) = n;
	}
	else
	{
		*root = n;
		return;
	}
	*get_parent(n) = *root;
}

/**
 * @brief fix the tree after insertion
 * 
 * @param root the root of the tree
 * @param k the node to be inserted
 */
void insert_fixup(block_ptr *root, block_ptr k)
{
	block_ptr u;

	while (*get_color(*get_parent(k)) == RED)
	{
		if (*get_parent(k) == *get_right_child(*get_parent(*get_parent(k))))
		{
			u = *get_left_child(*get_parent(*get_parent(k)));
			if (*get_color(u) == RED)
			{
				*get_color(u) = BLACK;
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(*get_parent(k))) = RED;
				k = *get_parent(*get_parent(k));
			}
			else
			{
				if (k == *get_left_child(*get_parent(k)))
				{
					k = *get_parent(k);
					rotate_right(k, root);
				}
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(*get_parent(k))) = RED;
				rotate_left(*get_parent(*get_parent(k)), root);
			}
		}
		else
		{
			u = *get_right_child(*get_parent(*get_parent(k)));
			if (*get_color(u) == RED)
			{
				*get_color(u) = BLACK;
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(*get_parent(k))) = RED;
				k = *get_parent(*get_parent(k));
			}
			else
			{
				if (k == *get_right_child(*get_parent(k)))
				{
					k = *get_parent(k);
					rotate_left(k, root);
				}
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(*get_parent(k))) = RED;
				rotate_right(*get_parent(*get_parent(k)), root);
			}
		}
		if (*root == k)
			break;
	}
	*get_color(*root) = BLACK;
}

/**
 * @brief return the minimum node of the tree
 * 
 * @param x the root of the tree
 * @return the minimum node of the tree 
 */
block_ptr minimum(block_ptr x)
{
	while (*get_left_child(x) != get_nil_node())
		x = *get_left_child(x);
	return (x);
}

/**
 * @brief fix the tree after deletion
 * 
 * @param x the pivot node
 * @param root the root of the tree
 */
void delete_fixup(block_ptr x, block_ptr *root)
{
	block_ptr s;

	while (x != *root && *get_color(x) == BLACK)
	{
		if (x == *get_left_child(*get_parent(x)))
		{
			s = *get_right_child(*get_parent(x));
			if (*get_color(s) == RED)
			{
				*get_color(s) = BLACK;
				*get_color(*get_parent(x)) = RED;
				rotate_left(*get_parent(x), root);
				s = *get_right_child(*get_parent(x));
			}
			if (*get_color(*get_left_child(s)) == BLACK && *get_color(*get_right_child(s)) == BLACK)
			{
				*get_color(s) = RED;
				x = *get_parent(x);
			}
			else
			{
				if (*get_color(*get_right_child(s)) == BLACK)
				{
					*get_color(*get_left_child(s)) = BLACK;
					*get_color(s) = RED;
					rotate_right(s, root);
					s = *get_right_child(*get_parent(x));
				}
				*get_color(s) = *get_color(*get_parent(x));
				*get_color(*get_parent(x)) = BLACK;
				*get_color(*get_right_child(s)) = BLACK;
				rotate_left(*get_parent(x), root);
				x = *root;
			}
		}
		else
		{
			s = *get_left_child(*get_parent(x));
			if (*get_color(s) == RED)
			{
				*get_color(s) = BLACK;
				*get_color(*get_parent(x)) = RED;
				rotate_right(*get_parent(x), root);
				s = *get_left_child(*get_parent(x));
			}
			if (*get_color(*get_right_child(s)) == BLACK && *get_color(*get_left_child(s)) == BLACK)
			{
				*get_color(s) = RED;
				x = *get_parent(x);
			}
			else
			{
				if (*get_color(*get_left_child(s)) == BLACK)
				{
					*get_color(*get_right_child(s)) = BLACK;
					*get_color(s) = RED;
					rotate_left(s, root);
					s = *get_left_child(*get_parent(x));
				}
				*get_color(s) = *get_color(*get_parent(x));
				*get_color(*get_parent(x)) = BLACK;
				*get_color(*get_left_child(s)) = BLACK;
				rotate_right(*get_parent(x), root);
				x = *root;
			}
		}
	}
	*get_color(x) = BLACK;
}

/**
 * @brief delete a node from the tree
 * 
 * @param z the node to delete
 * @param root the root of the tree
 */
void delete_node(block_ptr z, block_ptr *root)
{
	block_ptr y = z;
	block_ptr x;
	//int y_original_color = *get_color(y);
	if (*get_left_child(z) == get_nil_node())
	{
		x = *get_right_child(z);
		transplant(z, x, root);
	}
	else if (*get_right_child(z) == get_nil_node())
	{
		x = *get_left_child(z);
		transplant(z, x, root);
	}
	else
	{
		y = minimum(*get_right_child(z));
		//y_original_color = *get_color(y);
		x = *get_right_child(y);
		if (*get_parent(y) == z)
		{
			if (x != get_nil_node())
				*get_parent(x) = y;
		}
		else
		{
			transplant(y, *get_right_child(y), root);
			*get_right_child(y) = *get_right_child(z);
			*get_parent(*get_right_child(y)) = y;
		}
		transplant(z, y, root);
		*get_left_child(y) = *get_left_child(z);
		*get_parent(*get_left_child(y)) = y;
		*get_color(y) = *get_color(z);
	}
	//if (y_original_color == BLACK)
	//	delete_fixup(x, root);
}

/**
 * @brief count the number of nodes in the tree
 * 
 * @param root the root of the tree
 * @return size_t the number of nodes in the tree
 */
size_t count_nodes(block_ptr *root)
{
	size_t count = 0;
	if (*root != get_nil_node())
	{
		count++;
		count += count_nodes(get_left_child(*root));
		count += count_nodes(get_right_child(*root));
	}
	return (count);
}

/**
 * @brief Get the root of the tree by block size
 * 
 * @param size the block size
 * @return block_ptr* the root of the tree
 */
block_ptr *get_proper_root(size_t size)
{
	free_tree_t *free_trees = get_free_trees();

	if (size <= TINY_MAX_SIZE)
		return (&free_trees->tiny);
	else if (size <= SMALL_MAX_SIZE)
		return (&free_trees->small);
	else
		return (NULL);
}

/**
 * @brief find the root by where the node is
 * 
 * @param area the area start pointer
 * @param block the block
 * @return block_ptr* 
 */
block_ptr *find_proper_root(area_ptr area, block_ptr block)
{
	free_tree_t *free_trees = get_free_trees();

	if (block < area)
		return NULL;
	if (block < get_small_area(area))
		return (&free_trees->tiny);
	else if (block < get_large_area(area))
		return (&free_trees->small);
	return (NULL);
}

/**
 * @brief find the best fit block in the tree
 * 
 * @param size the size to fit
 * @param root the root of the tree 
 * @return block_ptr the best fit block
 */

block_ptr find_best_fit(size_t size, block_ptr *root)
{
	block_ptr best_fit = NULL;
	block_ptr current = *root;
	block_ptr const nil = get_nil_node();

	while (current != nil)
	{
		const size_t current_size = get_block_size(current);
		if (current_size == size)
			return (current);
		else if (current_size > size)
		{
			if (best_fit == NULL || current_size < get_block_size(best_fit))
				best_fit = current;
			current = *get_left_child(current);
		}
		else
			current = *get_right_child(current);
	}
	return (best_fit);
}

/**
 * @brief insert a free block into the tree
 * 
 * @param block the block to insert
 * @param root the root of the tree
 */
void insert_free_block(block_ptr block, block_ptr *root)
{
	if (is_allocated(block))
		return;

	*get_parent(block) = get_nil_node();
	*get_left_child(block) = get_nil_node();
	*get_right_child(block) = get_nil_node();
	*get_color(block) = RED;

	insert_recursive(root, block);

	/*if (*get_parent(block) == get_nil_node())
		*get_color(block) = BLACK;
	else
		insert_fixup(root, block);*/
}

/**
 * @brief delete a free block from the tree
 * 
 * @param block the block to delete
 * @param root the root of the tree 
 */
void delete_free_block(block_ptr block, block_ptr *root)
{
	if (is_allocated(block))
		return;

	delete_node(block, root);
}

size_t longest_branch_size(block_ptr root)
{
	size_t longest = 0;
	size_t current = 0;
	block_ptr const nil = get_nil_node();

	if (root == nil)
		return (0);
	if (*get_left_child(root) == nil && *get_right_child(root) == nil)
		return (1);
	current = longest_branch_size(*get_left_child(root));
	if (current > longest)
		longest = current;
	current = longest_branch_size(*get_right_child(root));
	if (current > longest)
		longest = current;
	return (longest + 1);
}