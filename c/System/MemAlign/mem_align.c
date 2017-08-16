#include <stdio.h>

#pragma pack(1)
struct s1 {
	char	d[6];
	int	e;
};

struct s2 {
	int	a;
	char	b;
	int	d;
	struct s1 s;
	short	c;
};

int main(void)
{
	struct s2 test1;
	struct s2 test2;

	printf("sizeof s2:%d\n", sizeof(struct s2));
	printf("addr of test 1:%X\n", &test1);
	printf("addr of test 2:%X\n", &test2);
	return 0;
}
