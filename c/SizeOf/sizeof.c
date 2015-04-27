#include<stdio.h>

struct S
{
	int j;
	char c;
};

int main(void)
{
	struct S s;
	struct S *ps;

	printf("sizeof int:%d\nsizeof long:%d\nsizeof long long:%d\n",
			sizeof(int), sizeof(long), sizeof(long long));

	ps	= &s;

	if (sizeof(int) > -1)
		printf("True\n");
	else
		printf("False\n");

	printf("%u\n", sizeof(int));
	printf("%u\n", -1);

	printf("size of s: %d\nsize of *ps:%d\nsize of ps:%d\nsize of ps->c:%d\nsize of ps->j:%d\n",			\
			sizeof(s), sizeof(*ps), sizeof(ps),								\
			sizeof(ps->c), sizeof(ps->j));
	return 0;
}
