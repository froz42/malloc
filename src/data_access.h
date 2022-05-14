#ifndef DATA_ACCESS_H
# define DATA_ACCESS_H

#include <stdlib.h>

size_t get_block_size(void *block);
void set_block_size(void *block, size_t size);
void *get_block_data(void *block);
void *get_next_block(void *block);
void **get_prev_block(void *block);

void *get_tiny_area(void *start);
void *get_small_area(void *start);
void *get_large_area(void *start);

void **get_left_child(void *block);
void **get_right_child(void *block);
void **get_parent(void *block);
int *get_color(void *block);
#endif // DATA_ACCESS_H
