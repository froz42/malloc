#ifndef RBTREE_H
# define RBTREE_H

#include "../malloc.h"

#define RED 1
#define BLACK 0

typedef struct free_tree_s
{
	block_ptr tiny;
	block_ptr small;
} free_tree_t;

block_ptr get_nil_node(void);
free_tree_t *get_free_trees(void);
void insert_free_block(block_ptr block);

#endif //RBTREE_H
