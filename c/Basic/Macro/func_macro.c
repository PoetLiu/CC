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

#define FUN_CALL_RETRY(func, t, ret) {	\
		{\
			int call_cnt = 0;\
			while (call_cnt <= t) {\
				ret = func;\
				if (ret != 0) {\
					usleep(500000);\
					call_cnt++;\
				} else {\
					break;\
				}\
			}\
		}\
}	

int test(char *msg) {
	printf("%s\n", msg);
	return -1;
}
int main(void)
{
	char *p = "hello, world!\n";
	int ret = 0;
	NF_HOOK(p, myfunc);
	FUN_CALL_RETRY(test("fuck"), 2, ret);
}


