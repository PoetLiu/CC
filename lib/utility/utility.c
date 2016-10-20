#include <stdio.h>
#include "utility.h"

static int _rand(void)
{
	static int first = 0;
	if (!first) {
		first	= 1;
		srand(time(NULL));
	}
	return rand();
}

int my_rand(int start, int end) 
{
	return start + _rand() % (end - start);
}


