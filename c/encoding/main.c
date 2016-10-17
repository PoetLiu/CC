#include <stdio.h>
#include <iconv.h>

int main(void)
{
	char *p = "\u7ebf", *in = NULL, *out = NULL;
	char buf[1024] = {0};
	iconv_t cd;
	int inlen = sizeof(p), outlen = 0;

	/*
	cd	= iconv_open("UTF-8", "UNICODE");
	in	= p;
	out	= buf;
	iconv(cd, &in, (size_t*)&inlen, &out, &outlen);
	printf("len:%d p:%s str:%s\n", outlen, p, buf);
	*/
	snprintf(buf, sizeof(buf), "%s", p);
	printf("len:%d p:%s str:%s\n", outlen, p, buf);
	iconv_close(cd);
	return 0;
}
