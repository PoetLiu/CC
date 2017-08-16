#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("my process group id :%d\n", getpgrp());
	printf("my process id :%d\n", getpid());
	return 0;
}
