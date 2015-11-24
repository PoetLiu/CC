#include <stdio.h>

int crash_func(int i, int j)
{
	int ret = 0;
	ret	= i / j;
	return ret;
}

int main(void)
{
	crash_func(5, 0);
	return 0;
}
