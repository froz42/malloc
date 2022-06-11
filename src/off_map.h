/**
 * @file off_map.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief header file for off map allocation
 * @date 2022-06-06
 *
 */

#ifndef OFF_MAP_H
#define OFF_MAP_H

#include "malloc.h"

block_ptr *get_off_map_list(void);
block_ptr new_off_map_block(size_t size);
void remove_off_map_block(block_ptr block);
size_t get_off_map_size(void);

#endif