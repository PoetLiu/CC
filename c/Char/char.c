#include <stdio.h>

int main(void)
{
	char	c = -1;

	if ( c < 200) {
		printf("signed %d:%c\n", c, c);
	} else {
		printf("unsigned %d:%c\n", c, c);
	}

	return 0;
}
