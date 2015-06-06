#include <stdio.h>

void myfunc(unsigned char *buf)
{
	printf(buf);
}

static inline int 
NF_HOOK(unsigned char *buf, void (*okfn)(unsigned char *buf1)) {
	okfn(buf);
	return 1;
}

#define NF_HOOK(buf, okfn) (okfn)(buf)

int main(void)
{
	char *p = "hello, world!\n";
	NF_HOOK(p, myfunc);
}


