#include <stdio.h>
#include <stdlib.h>
#include "bin_2_str.h"

int main(void) 
{
	uchar a[] = {
		0x46,
		0xF2,
		0x43,
		0xD2,
		0x1A,
		0x60,
		0x28,
		0x6F,
		0xD0,
		0xF1,
		0x81,
		0x6A,
		0x61,
		0x8A,
		0x7F,
		0x9C
	};

	char *str;

	str = bin_2_str(a, sizeof(a));
	printf("%s\n", str);
	free(str);
	return 0;
}
