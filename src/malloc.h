#ifndef MALLOC_H
# define MALLOC_H

# ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS	0x20
# endif

typedef void* block_ptr;
typedef void* area_ptr;

# include "data_access.h"
# include "rbtree/rbtree.h"
# include "split_block.h"
# include "off_map.h"
# include "unfrag.h"

# define TINY_MAX_SIZE 256 // max size in bytes for tiny area
# define SMALL_MAX_SIZE 1024 // max size in bytes for small area

// capacity in bytes for tiny area
# define TINY_CAPACITY (TINY_MAX_SIZE * 256 + sizeof(size_t) * 256 * 4)

// capacity in bytes for small area
# define SMALL_CAPACITY (SMALL_MAX_SIZE * 256 + sizeof(size_t) * 256 * 4)

# define TOTAL_CAPACITY (TINY_CAPACITY + SMALL_CAPACITY)

# define ALLIGN_16(x) ((x + 15) & ~15)

// minimal size is the size minimal to store rbtree node
# define MINIMAL_SIZE ALLIGN_16(sizeof(size_t) + (sizeof(void *) * 4) + sizeof(int))

void init_area(area_ptr area, size_t size);
area_ptr get_or_create_area(void);
void *ft_malloc(size_t size);
void ft_free(void *data);

#endif