/**
 * @file malloc.h
 * @author tmatis (tmatis@student.42.fr)
 * @brief malloc header file
 * @date 2022-06-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MALLOC_H
# define MALLOC_H

# ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS	0x20
# endif

// check is REAL_MALLOC is defined
# ifndef TEST_MALLOC
#  define MALLOC_NAME malloc
#  define FREE_NAME free
#  define REALLOC_NAME realloc
#  define CALLOC_NAME calloc
#else
#  define MALLOC_NAME ft_malloc
#  define FREE_NAME ft_free
#  define REALLOC_NAME ft_realloc
#  define CALLOC_NAME ft_calloc
#endif

/**
 * @brief A block of memory
 * The block has the following structure:
 * ---------------------------------
 * | size | prev | data + padding |
 * ---------------------------------
 * size: the size of the block in bytes
 * prev: the previous block in the area
 * data + padding: the data of the block
 * The data of the block is aligned to 16 bytes 
 * 
 * The off map block has the following structure:
 * ----------------------------------------
 * | size | prev | data + padding | next |
 * ----------------------------------------
 * size: the size of the block in bytes
 * prev: the previous block in the area
 * data + padding: the data of the block
 * next: the next block in the area (only for off map blocks)
 * 
 * The rbtree is a red-black tree.
 * It is used to store free blocks and to find the best fit block.
 * The rbtree is implemented as a binary tree.
 * The nil node points to itself.
 * the block structure is as follows:
 * ----------------------------------------------------------
 * | size | prev | left | right | parent | padding |  color |
 * ----------------------------------------------------------
 * The size field is the size of the block.
 * The prev field points to the previous block.
 * The left field points to the left child.
 * The right field points to the right child.
 * The parent field points to the parent node.
 * The color field is used to indicate the color of the node.
 * The color field is either RED or BLACK.
 */
typedef void* block_ptr;

/**
 * @brief Area ptr
 */
typedef void* area_ptr;

# include "utils.h"
# include "data_access.h"
# include "area_utils.h"
# include "rbtree/rbtree.h"
# include "block_manipulation.h"
# include "off_map.h"
# include "config.h"

# define TINY_MAX_SIZE 256 // max size in bytes for tiny area
# define SMALL_MAX_SIZE 1024 // max size in bytes for small area

// capacity in bytes for tiny area
# define TINY_CAPACITY (TINY_MAX_SIZE * TINY_MAX_SIZE + sizeof(size_t) * TINY_MAX_SIZE * 2)

// capacity in bytes for small area
# define SMALL_CAPACITY (SMALL_MAX_SIZE * SMALL_MAX_SIZE + sizeof(size_t) * SMALL_MAX_SIZE * 2)

# define TOTAL_CAPACITY (TINY_CAPACITY + SMALL_CAPACITY)

# define ALLIGN_16(x) ((x + 15) & ~15)
# define ALLIGN_PAGE(x) ((x + getpagesize() - 1) & ~(getpagesize() - 1))

// minimal size is the size minimal to store rbtree node
# define MINIMAL_SIZE ALLIGN_16((sizeof(void *) * 3) + sizeof(int))

void init_area(area_ptr area, size_t size);
area_ptr get_or_create_area(void);
void error_write(char *str);
void show_alloc_mem_ex(void);
void dump_block(block_ptr block);

void *ft_malloc(size_t size);
void ft_free(void *data);
void *ft_realloc(void *ptr, size_t size);
void *ft_calloc(size_t nmemb, size_t size);
size_t sum_of_block(void *area_start, void *area_end);

#endif