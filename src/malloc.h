#ifndef MALLOC_H
# define MALLOC_H

# ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS	0x20
# endif


# define TINY_MAX_SIZE 256 // max size in bytes for tiny area
# define SMALL_MAX_SIZE 1024 // max size in bytes for small area

// capacity in bytes for tiny area
# define TINY_CAPACITY (TINY_MAX_SIZE * 256 + sizeof(size_t) * 256 * 2)

// capacity in bytes for small area
# define SMALL_CAPACITY (SMALL_MAX_SIZE * 256 + sizeof(size_t) * 256 * 2)

# define TOTAL_CAPACITY (TINY_CAPACITY + SMALL_CAPACITY)  

void *get_or_create_area();


#endif