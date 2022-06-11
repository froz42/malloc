/**
 * @file block_manipulation.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief header file for various block manipulation functions
 * @date 2022-06-06
 * 
 */
#ifndef BLOCK_MANIPULATION_H
# define BLOCK_MANIPULATION_H

#include "malloc.h"

block_ptr split_block(block_ptr block, size_t size, void *areaend);
void merge_next_block(block_ptr block_a, void *areaend);
block_ptr unfrag_block(block_ptr block, void *area_end, block_ptr *root);
int extend_block(block_ptr block, size_t wanted_size, area_ptr area);
int shrink_block(block_ptr block, size_t wanted_size, area_ptr area);
#endif