#include <stdio.h>
#include <stdlib.h>
int 
main(void)
{
	int	c;

	while ((c = getc(stdin)) != EOF)
		if (putc(c, stdout) == EOF)
			printf("output error\n");

	if (ferror(stdin))
		printf("input error\n");

	exit(0);
}
