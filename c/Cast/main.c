#include <stdio.h>

int main(void) 
{
	//char	c = '\xff';
	char	c = -1;


	printf("signed char 'FF' to uint32_t:%x\n", c);
	printf("signed char 'FF' to int32_t:%d\n", c);
	printf("unsigned char 'FF' to uint32_t:%d\n", (unsigned char)c);
	printf("unsigned char 'FF' to int32_t:%d\n", (unsigned char)c);


	return 0;
}
