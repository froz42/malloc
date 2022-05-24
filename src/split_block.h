#ifndef SPLIT_BLOCK_H
# define SPLIT_BLOCK_H

#include "malloc.h"

block_ptr split_block(block_ptr block, size_t size, void *areaend);

#endif