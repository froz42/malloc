#ifndef RBTREE_H
# define RBTREE_H

#include "../malloc.h"

#define RED 1
#define BLACK 0

typedef struct free_tree_s
{
	void *tiny;
	void *small;
} free_tree_t;

void *get_nil_node(void);
void init_rb_node(void *block);
free_tree_t *get_free_trees(void);

#endif //RBTREE_H
