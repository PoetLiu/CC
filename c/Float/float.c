#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	unsigned char f1[4] = {0, 0, 251, 66};
	unsigned char f2[4] = {66, 251, 0, 0};

	printf("f1:%f\nf2:%f\n", *((float *)f1), *((float *)f2));

	char *f[] = {"2.5", "2.45", "2.445", "2.4445"};
	int i = 0;
	unsigned long d;
	char buf[10] = {0};

	for (i = 0; i < sizeof(f)/sizeof(char *); i++) {
		snprintf(buf, sizeof(buf), "%.1lf", atof(f[i]));
		printf("%s ", buf);
		printf("%.1lf ", atof(buf));
		d	= atof(f[i]) * 128.0;
		printf("%lf\n", d / 128.0);
	}

	return 0;
}
