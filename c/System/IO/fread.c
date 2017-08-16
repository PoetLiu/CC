#include <stdio.h>
#include <stdlib.h>

typedef struct item{
	short count;
	long total;
	char name[100];
}item;

int main(void)
{
	FILE *fp;
	float data[10];
	int i;
	item j;

	if ((fp = fopen("./data", "r")) == NULL)
		printf("fopen error\n");

	if ((fread(data, sizeof(float), 4, fp)) != 4)
		printf("fread error 1\n");

	if ((fread(&j, sizeof(j), 1, fp)) != 1)
		printf("fread error 2\n");

	for (i = 0;i < 4; i++)
		printf("%f ", data[i]);

	printf("\n");
	printf("%d %ld %s", j.count, j.total, j.name);

	exit(0);
}

