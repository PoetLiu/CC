#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void) 
{
	char *str = "hello, world!\n";
	if( write(1, str, strlen(str)) != strlen(str))
		printf("write error:%s\n", strerror(errno));
	return 0;
}
