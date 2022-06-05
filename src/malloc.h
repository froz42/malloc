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
# define TINY_CAPACITY (TINY_MAX_SIZE * TINY_MAX_SIZE + sizeof(size_t) * TINY_MAX_SIZE * 2)

// capacity in bytes for small area
# define SMALL_CAPACITY (SMALL_MAX_SIZE * SMALL_MAX_SIZE + sizeof(size_t) * SMALL_MAX_SIZE * 2)

# define TOTAL_CAPACITY (TINY_CAPACITY + SMALL_CAPACITY)

# define ALLIGN_16(x) ((x + 15) & ~15)

// minimal size is the size minimal to store rbtree node
# define MINIMAL_SIZE ALLIGN_16(sizeof(size_t) + (sizeof(void *) * 4) + sizeof(int))

void init_area(area_ptr area, size_t size);
area_ptr get_or_create_area(void);
void error_write(char *str);
void fancy_memory_dump(void);

void *ft_malloc(size_t size);
void ft_free(void *data);
void *ft_realloc(void *ptr, size_t size);
void *ft_calloc(size_t nmemb, size_t size);
size_t sum_of_block(void *area_start, void *area_end);
void check_prev(void *area_start, void *area_end);

#endif