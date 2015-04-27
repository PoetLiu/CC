#include <stdio.h>

char a[10][10] =
{
	"helloworld",
	"helloworl"
};

int main(void)
{
	printf("a=%x\na+1=%x\na[1]+1=%x\n*a=%x\n&a[1][1]=%x\n&a[1]=%x\n", a, a+1, a[1]+1, *a, &a[1][1], &a[1]);
	printf("%s:%d:%s\n", __FUNCTION__, __LINE__, __FILE__);
	return 0;
}
