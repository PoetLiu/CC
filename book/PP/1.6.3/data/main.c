#include <stdio.h>
#include "data.h"
#include "mem.h"

int main(void)
{
	FILE *fp = NULL;
	int i = 0;

	fp	= fopen("./data.in", "w+");
	P_VALID_CHECK_RET(fp, -1);
	
	for (i = 0; i < DATA_NUM; i++) {
		fprintf(fp, "%d\n", i);
	}

	fclose(fp);
	return 0;
}
