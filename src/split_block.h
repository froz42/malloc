/**
 * @file split_block.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief header file for split block and defragmentation
 * @date 2022-06-06
 * 
 */
#ifndef SPLIT_BLOCK_H
# define SPLIT_BLOCK_H

#include "malloc.h"

block_ptr split_block(block_ptr block, size_t size, void *areaend);
void merge_next_block(block_ptr block_a, void *areaend);
block_ptr unfrag_block(block_ptr block, void *area_end, block_ptr *root);
int extend_block(block_ptr block, size_t wanted_size, area_ptr area);
#endif