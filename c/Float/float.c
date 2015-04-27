#include <stdio.h>

int main(void)
{
	unsigned char f1[4] = {0, 0, 251, 66};
	unsigned char f2[4] = {66, 251, 0, 0};

	printf("f1:%f\nf2:%f\n", *((float *)f1), *((float *)f2));

	return 0;
}
