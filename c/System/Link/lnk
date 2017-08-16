#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
	if (link("./test1", "./test2") != 0) { 
		printf("link error: %s\n", strerror(errno));
	}else
		printf("link ok\n");

	return 0;
}
