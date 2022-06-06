/**
 * @file data_access.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief Data access header file
 * @date 2022-06-06
 * 
 */
#ifndef DATA_ACCESS_H
# define DATA_ACCESS_H

#include <stdlib.h>
#include "malloc.h"

size_t get_block_size(block_ptr block);
void set_block_size(block_ptr block, size_t size);
void *get_block_data(block_ptr block);
void set_raw_block_size(block_ptr block, size_t size);

block_ptr get_next_block(block_ptr block);
block_ptr *get_prev_block(block_ptr block);
block_ptr *get_off_map_next_block(block_ptr block);

area_ptr get_tiny_area(void *start);
area_ptr get_small_area(void *start);
area_ptr get_large_area(void *start);

block_ptr *get_left_child(block_ptr block);
block_ptr *get_right_child(block_ptr block);
block_ptr *get_parent(block_ptr block);
int *get_color(void *block);

int is_allocated(block_ptr block);
void set_allocated(block_ptr block);
void set_free(block_ptr block);

block_ptr get_block_from_data(void *data);

#endif // DATA_ACCESS_H
