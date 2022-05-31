#include <stdlib.h>
#include "rbtree.h"
#include <stdio.h>

/*
** this function return the sentinel node
*/
block_ptr get_nil_node(void)
{
	static char nil_node[MINIMAL_SIZE];
	static int is_init = 0;

	if (is_init == 0)
	{
		init_area(nil_node, MINIMAL_SIZE);
		*get_parent(nil_node) = nil_node;
		*get_left_child(nil_node) = nil_node;
		*get_right_child(nil_node) = nil_node;
		*get_color(nil_node) = BLACK;
		is_init = 1;
	}
	return (nil_node);
}

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
		return ;
	}
	*get_parent(n) = *root;
}

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
			break ;
	}
	*get_color(*root) = BLACK;
}

block_ptr minimum(block_ptr x)
{
	while (*get_left_child(x) != get_nil_node())
		x = *get_left_child(x);
	return (x);
}

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

void delete_node(block_ptr z, block_ptr *root)
{
	block_ptr y = z;
	block_ptr x;
	int y_original_color = *get_color(y);
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
		y_original_color = *get_color(y);
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
	if (y_original_color == BLACK)
		delete_fixup(x, root);
}

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

void insert_free_block(block_ptr block, block_ptr *root)
{
	if (is_allocated(block))
		return ;

	*get_parent(block) = get_nil_node();
	*get_left_child(block) = get_nil_node();
	*get_right_child(block) = get_nil_node();
	*get_color(block) = RED;

	insert_recursive(root, block);

	if (*get_parent(block) == get_nil_node())
		*get_color(block) = BLACK;
	else
		insert_fixup(root, block);
}

void delete_free_block(block_ptr block, block_ptr *root)
{
	if (is_allocated(block))
		return ;

	delete_node(block, root);
}
