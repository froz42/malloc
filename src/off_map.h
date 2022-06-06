/**
 * @file off_map.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief header file for off map allocation
 * @date 2022-06-06
 * 
 */

#ifndef OFF_MAP_H
# define OFF_MAP_H

# include "malloc.h"

block_ptr new_off_map_block(size_t size);
void remove_off_map_block(block_ptr block);

#endif