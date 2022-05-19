#ifndef FIT_ALLOCATION_H
# define FIT_ALLOCATION_H

# include <stdlib.h>
# include "malloc.h"

int is_allocated(block_ptr block);
void set_allocated(block_ptr block);
block_ptr search_fit(area_ptr area, size_t size, size_t capacity);
block_ptr find_fit(area_ptr start, size_t size);

#endif