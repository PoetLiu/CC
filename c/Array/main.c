#include <stdio.h>

int a[4][5];

char p1[10]	= "he\0ll\0o"; 
char (*ch1)[3] = &p1;

int main(void)
{
	int i, j, counter;

	for (i = 0, counter = 0; i < 4; i++)
		for (j = 0; j < 5; j++, counter++)
			a[i][j]	= counter;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 5; j++)
			printf("%d ", a[i][j]);
	printf("\n");

	int (*p)[5];
	p	= a;
	for (i = 0; i < 5; i++)
		printf("%d ", *(*p + i));

	printf("\n");

	printf("%d\n", **ch1);
	printf("%s\n", *ch1);
	printf("%s\n", ch1);
	printf("%s\n", ch1+1);
	printf("%s\n", ch1+2);
	printf("p1:%p p1+1:%p\n", p1, p1 + 1);
	return 0;
}
