#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float myroundf(float f, int n)
{
	char buf[128] = {0};	
	int n_str, n_deci, i, j;	// 小数的位数
	char *p = NULL;

	n_str	= snprintf(buf, sizeof(buf), "%f", f);
	p	= strstr(buf, ".");
	for (i = 1; p[i] != NULL; i++)
		if (p[i] < '4')
			break;
	n_deci	= i;

	if (n >= n_deci)
		return f;

	for (i = n_deci; i > 0; i++) {
					
	}
	return atof(buf);
}

double myroundd(double d, int n)
{

}

int main()
{
	int i = 0;
	float f[]	= {
		1.5, 1.45, 1.445, 1.4445, 1.44445,
		1.4, 1.44, 1.444, 1.4444, 1.44444
	};

//	for (i = 0; i < sizeof(f)/sizeof(float); i++)
		printf("%f\n", myroundf(f[3], 1));

	return 0;
}
