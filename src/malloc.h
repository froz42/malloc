#ifndef MALLOC_H
# define MALLOC_H

# ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS	0x20
# endif

# include "data_access.h"

# define TINY_MAX_SIZE 256 // max size in bytes for tiny area
# define SMALL_MAX_SIZE 1024 // max size in bytes for small area

// capacity in bytes for tiny area
# define TINY_CAPACITY (TINY_MAX_SIZE * 256 + sizeof(size_t) * 256 * 4)

// capacity in bytes for small area
# define SMALL_CAPACITY (SMALL_MAX_SIZE * 256 + sizeof(size_t) * 256 * 4)

# define TOTAL_CAPACITY (TINY_CAPACITY + SMALL_CAPACITY)

# define MINIMAL_SIZE sizeof(size_t) + (sizeof(void *) * 4)

# define ALLIGN_16(x) ((x + 15) & ~15)

void *get_or_create_area();


#endif