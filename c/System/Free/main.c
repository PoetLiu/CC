#include <stdio.h>

char *post_msg(char *msg) 
{
	char *ret = "Hello, World!";
	return ret;	
}

int main(void)
{
	
	char *ret = post_msg(NULL);
	free(ret);
	return 0;
}
