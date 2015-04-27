#include <stdio.h>

int	i;

void func(int j)
{
	printf("before get new value:%d\n", i);
	i	= j;
	printf("after get new value:%d\n", i);
}
