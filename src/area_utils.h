/**
 * @file area_utils.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief area utils header file
 * @date 2022-06-06
 * 
 */

#ifndef AREA_UTILS_H
#define AREA_UTILS_H

#include "malloc.h"

void init_area(area_ptr area, size_t size);
area_ptr get_or_create_area(void);
area_ptr find_area_end(area_ptr area, block_ptr block);
void *handle_off_map(size_t size);
int is_off_map(void *data, area_ptr area);
size_t get_minimal_size(block_ptr block, area_ptr area);
size_t get_maximal_size(block_ptr block, area_ptr area);
#endif