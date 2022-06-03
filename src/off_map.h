#ifndef OFF_MAP_H
# define OFF_MAP_H

# include "malloc.h"

block_ptr new_off_map_block(size_t size);
void remove_off_map_block(block_ptr block);

#endif