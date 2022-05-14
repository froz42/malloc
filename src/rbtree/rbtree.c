#include <stdlib.h>
#include "rbtree.h"


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

/*
void init_rb_node(void *block)
{
	
}
*/


