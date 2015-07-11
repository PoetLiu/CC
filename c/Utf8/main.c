#include<stdio.h>
#include<string.h>

int main(void)
{
	char *p = "我是谁";
	int i;

	printf("bin value for %s:\n", p);
	for (i = 0; i < strlen(p); i++)
		printf("%.2x ", *(unsigned char *)&p[i]);
	printf("\n");
	return 0;
}
