#include <stdlib.h>
#include "rbtree.h"
#include <stdio.h>

/*
** this function return the sentinel node
*/
void *get_nil_node(void)
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

void *get_grand_parent(void *block)
{
	if (is_allocated(block))
	{
		printf("error: grand_parent: block is allocated\n");
		exit(EXIT_FAILURE);
	}
	return (*get_parent(*get_parent(block)));
}

void *get_sibling(void *block)
{
	if (is_allocated(block))
	{
		printf("error: get_sibling: block is allocated\n");
		exit(EXIT_FAILURE);
	}
	if (*get_parent(block) == *get_left_child(*get_parent(block)))
		return (*get_right_child(*get_parent(block)));
	else
		return (*get_left_child(*get_parent(block)));
}

void rotate_left(void *x, void **root)
{
	void *y = *get_right_child(x);
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

void rotate_right(void *x, void **root)
{
	void *y = *get_left_child(x);
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

void insert_recursive(void **root, void *n)
{
	if (*root != get_nil_node() && get_block_size(*root) < get_block_size(n))
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
	*get_parent(n) = *root;
	*get_color(n) = RED;
	*get_left_child(n) = get_nil_node();
	*get_right_child(n) = get_nil_node();
}

void insert_fixup(void *k, void **root)
{
	void *u;

	while (*get_color(*get_parent(k)) == RED)
	{
		if (*get_parent(k) == *get_right_child(*get_parent(get_parent(k))))
		{
			u = *get_left_child(*get_parent(get_parent(k)));
			if (*get_color(u) == RED)
			{
				*get_color(u) = BLACK;
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(get_parent(k))) = RED;
				k = *get_parent(get_parent(k));
			}
			else
			{
				if (k == *get_left_child(*get_parent(k)))
				{
					k = *get_parent(k);
					rotate_right(k, root);
				}
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(get_parent(k))) = RED;
				rotate_left(*get_parent(get_parent(k)), root);
			}
		}
		else
		{
			u = *get_right_child(*get_parent(get_parent(k)));

			if (*get_color(u) == RED)
			{
				*get_color(u) = BLACK;
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(get_parent(k))) = RED;
				k = *get_parent(get_parent(k));
			}
			else
			{
				if (k == *get_right_child(*get_parent(k)))
				{
					k = *get_parent(k);
					rotate_left(k, root);
				}
				*get_color(*get_parent(k)) = BLACK;
				*get_color(*get_parent(get_parent(k))) = RED;
				rotate_right(*get_parent(get_parent(k)), root);
			}
		}
		if (*root == k)
			break;
	}
	*get_color(*root) = BLACK;
}

void insert_free_block(void *block)
{
	if (is_allocated(block))
	{
		printf("error: insert_free_block: block is allocated\n");
		return ;
	}

	*get_parent(block) = get_nil_node();
	*get_left_child(block) = get_nil_node();
	*get_right_child(block) = get_nil_node();
	*get_color(block) = RED;

	void **root;
	free_tree_t *free_trees = get_free_trees();

	if (get_block_size(block) <= TINY_MAX_SIZE)
		root = &free_trees->tiny;
	else if (get_block_size(block) <= SMALL_MAX_SIZE)
		root = &free_trees->small;
	else
	{
		printf("error: insert_free_block: block is too large\n");
		return ;
	}
	
	insert_recursive(root, block);

	if (*get_parent(block) == get_nil_node())
		*get_color(block) = BLACK;
	else
		insert_fixup(root, block);
}

