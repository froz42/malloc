#ifndef UNFRAG_H
# define UNFRAG_H

# include "malloc.h"

block_ptr unfrag_block(block_ptr block, void *area_end, block_ptr *root);

#endif