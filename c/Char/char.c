#include <stdio.h>

//unsigned -1:▒


int main(void)
{
	char	c = -1;

	if (c < 200) {
		printf("signed %d:%c\n", c, c);
	} else {
		printf("unsigned %d:%c\n", c, c);
	}

	if (c == -1) {
		printf("equal\n");
	} else {
		printf("not equal\n");
	} 
	return 0;
}
