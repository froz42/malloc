#ifndef DATA_ACCESS_H
# define DATA_ACCESS_H

#include <stdlib.h>

size_t get_block_size(void *block);
void *get_block_data(void *block);
void *get_next_block(void *block);
void **get_prev_block(void *block);

void *get_tiny_area(void *start);
void *get_small_area(void *start);
void *get_large_area(void *start);

#endif // DATA_ACCESS_H
