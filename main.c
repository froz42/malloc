#include <stdlib.h>
#include <unistd.h>

#define n_block 20

int main(void)
{
	void *allocs[n_block / 2];
	malloc(1024);
	sleep(2);
	for (int i = 0; i < n_block / 2; i++)
	{
		malloc(512);
		allocs[i] = malloc(512);
	}
	for (int i = 0; i < n_block / 2; i++)
	{
		free(allocs[i]);
	}

	while (1)
	{
	}
}