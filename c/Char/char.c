#include <stdio.h>

//unsigned -1:▒


int main(void)
{
	char	c = -1;

	if ( c < 200UL) {
		printf("signed %d:%c\n", c, c);
	} else {
		printf("unsigned %d:%c\n", c, c);
	}

	return 0;
}
