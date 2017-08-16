#include<stdio.h>

int main(void)
{
	FILE *fp;
	int c;

	fp	= fopen("test.txt", "r");
	c 	= fgetc(fp);

	while(!feof(fp)) {
		printf("%c", c);
		c = fgetc(fp);
	}
	fclose(fp);

	return 0;
}
