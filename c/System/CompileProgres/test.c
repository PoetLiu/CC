#include<stdio.h>

int main(void)
{
	printf("%s:%s:%d:hello, world!\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}
