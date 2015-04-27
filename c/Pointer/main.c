#include <stdio.h>

typedef struct s_7{
	char p[16];
}*S_7;

int main(void)
{
	double *p1 = (double *)100;
	double *p2 = (double *)117;
	S_7 	p3 = (S_7)100;
	S_7	p4 = (S_7)117;

	printf("p1-p2=%d\n", p1 - p2);
	printf("p2-p1=%d\n", p2 - p1);

	printf("sizeof S_7:%d\n", sizeof(struct s_7));

	printf("p3-p4=%d\n", p3 - p4);
	printf("p4-p3=%d\n", p4 - p3);
	return 0;
	
}
