#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAC_SIZE	6

int str2mac(unsigned char *mac, const char *str)
{
	int ret, i;
	
	// warning: we can't define a unsigned char tmp array
	// because %x need a unsigned int * value not unsigned char *
	// int take 4 Bytes but char 1 Bytes, so it will cause overflow while write value to tmp[5]
	unsigned int tmp[MAC_SIZE] = {0};
	ret = sscanf(str, "%2x%*[-|:]%2x%*[-|:]%2x%*[-|:]%2x%*[-|:]%2x%*[-|:]%2x%*[-|:]", \
			&tmp[0], &tmp[1] ,&tmp[2], &tmp[3], &tmp[4], &tmp[5]);
	if (ret != MAC_SIZE)
		return -1;

	for (i = 0; i < MAC_SIZE; i++)
		mac[i]	= tmp[i];
	return 0;
}

int main(void)
{
	unsigned char mac[MAC_SIZE] = {0};	
	char *m1	= "01-0C-98-68-cd-80";
	char *m2	= "11:1C:A8:78:dd:90";
	int i;
	
	memset(mac, 0xff, sizeof(mac));

	str2mac(mac, m2);

	for (i = 0; i < MAC_SIZE; i++) {
		printf("%02x", mac[i]);
		if (i != MAC_SIZE - 1)
			printf(":");
	}
	printf("\n");
	return 0;
}
