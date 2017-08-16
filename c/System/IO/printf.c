#include <stdio.h>

int main(void)
{
	unsigned int counter;

	counter	= 0;

	printf("hello%n\n", &counter);
	printf("counter:%d\n", counter);

	return 0;
}
