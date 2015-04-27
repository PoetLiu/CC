#include <stdio.h>

typedef struct item{
	short count;
	long total;
	char name[100];
} item;

int main(void)
{
	FILE *fp;
	item i = {
		15, 123, "hello, world\n"
	};

	float data[10] = {
		1.1111f, 2.2222f, 3.3333f, 4.4444f, 5.5555f,
		6.0f, 7.0f, 8.0f, 9.0f, 10.0f
	};

	if ((fp = fopen("./data", "w")) == NULL)
		printf("fopen error\n");

	if (fwrite(&data[2], sizeof(float), 4, fp) != 4)
		printf("fwrite error 1\n");

	if (fwrite(&i, sizeof(i), 1, fp) != 1)
		printf("fwrite error 2\n");

	printf("%d %ld %s", i.count, i.total, i.name);
	close(fp);
}
