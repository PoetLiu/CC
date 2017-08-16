#include <stdio.h>
#include <arpa/inet.h>

int main(void) 
{

#if __BYTE_ORDER == __LITTLE_ENDIAN
	printf("little_endian\n");
#elif __BYTE_ORDER == __BIG_ENDIAN
	printf("big_endian\n");
#endif
	unsigned short i = 500;
	unsigned char a = *(unsigned char *)&i;
	unsigned char b = *((unsigned char *)&i + 1);
	printf("a:%x, b:%x\n", a, b);

	unsigned short j = htons(i);
	unsigned char c = *(unsigned char *)&j;
	unsigned char d = *((unsigned char *)&j + 1);
	printf("c:%x, d:%x\n", c, d);
	
	return 0;
}
