#include <stdio.h>
#include "md5.h"

int main(void)
{
	struct MD5Context md5;
	char s[] = "helloworld";
	unsigned char ss[16];
	int i = 0;
	char buf[128] = {0};

	MD5Init(&md5);
	MD5Update(&md5, s, strlen(s));
	MD5Final(ss, &md5);

	for (i = 0; i < sizeof(ss); i++) {
		sprintf(buf + 2*i, "%02X", ss[i]);	
	}

	printf("%s\n", buf);
	
	return 0;
}
