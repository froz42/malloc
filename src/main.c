#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

int main(void)
{
	void *data = ft_malloc(60);
	printf("%p\n", data);
}
