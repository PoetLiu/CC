/*
 *
 * Authored by Liu Peng on: Fri Nov 21 10:55:44 CST 2014
 *
 * @desc: 内存对齐
 *
 * @func:
**/
#include <stdio.h>

struct aa{
	int	a;
	char	b;
	short	c;
	char	d;
}__attribute__((packed));

struct bb{
	int	a;
	char 	b;
	short	c;
	char	d;
};

struct cc{
	char 	d:1;
	int	a:1;
	char	b:1;
	short	c:1;
};

struct dd{
	int	ci;
	char	c[7];
};
int main(void)
{
	struct aa a;
	struct bb b;
	struct cc c;
	struct dd d;
	printf("sizeof aa %d\n", sizeof(a));
	printf("sizeof bb %d\n", sizeof(b));
	printf("sizeof cc %d\n", sizeof(c));
	printf("sizeof dd %d\n", sizeof(d));
	return 0;
}
