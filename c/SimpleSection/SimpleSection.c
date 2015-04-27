/*
 *SimpleSection.c  
 *
 *Linux:
 *	gcc -c SimpleSection.c
 *
 *Windows:
 *	c1 SimpleSection.c /c /za
 */

#include<stdio.h>

int printf(const char *format,...);
int global_init_vat	= 84;
int global_uninit_var;

void func1( int i )
{
	printf("%d\n", i);
}

int main(void)
{
	static int static_var	= 85;
	static int static_var2;
	int a	= 1;
	int b;

	func1( static_var + static_var2 + a + b );

	return a;
}
