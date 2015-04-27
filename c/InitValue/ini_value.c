#include<stdio.h>

int main(void)
{
	int a;
	static int b;

	a	= b, b++, a++; 
	printf("a = %d, b = %d\n", a, b);

	// use '=' to test
	if(a = 0)
		printf("I am in if\n");
	return 0;
}
