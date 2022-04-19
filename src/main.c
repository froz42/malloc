#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

int main(void)
{
	printf("getpagesize() -> %d\n", getpagesize());
	printf("TINY_CAPACITY -> %lu\n", TINY_CAPACITY);
	printf("SMALL_CAPACITY -> %lu\n", SMALL_CAPACITY);
	printf("sizeof(size_t) -> %lu\n", sizeof(size_t));
	printf("sizeof(void *) -> %lu\n", sizeof(void *));

}
