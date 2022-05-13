#ifndef FIT_ALLOCATION_H
#define FIT_ALLOCATION_H

#include <stdlib.h>

int is_allocated(void *block);
void *search_fit(void *area, size_t size, size_t capacity);
void *find_fit(void *start, size_t size);

#endif