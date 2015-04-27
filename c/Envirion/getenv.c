#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("HOME	: %s\n", getenv("HOME"));
	printf("LANG	: %s\n", getenv("LANG"));
	printf("PWD	: %s\n", getenv("PWD"));

	return 0;
}
