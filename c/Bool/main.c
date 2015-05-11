/*
 *
 * Authored by Liu Peng on: Thu Apr 30 17:56:06 CST 2015
 *
 * @desc: negative and positive value is true when be done logic operation
 * 	  zero(0) is false
 *
 * @func:
**/


#include <stdio.h>

int main(void)
{

	int i;
	int d[]	= {
		-1, 0, 1
	};
	
	for (i = 0; i < 3; i++) {
		if (!d[i])
			printf("d[%d] is %d false\n", i, d[i]);
		else
			printf("d[%d] is %d true\n", i, d[i]);

	}

	return 0;
}
