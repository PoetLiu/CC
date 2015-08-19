#include <stdio.h>
#include <string.h>
#include "mem.h"
#include "debug.h"

static int utf8_chinese_check(const char *str)
{
        int i = 0;
        P_VALID_CHECK_ACT(str, goto f);
        for (i = 0; i < strlen(str); i++) {
                if (str[i]&0x80)
                        goto t;
        }

f:// hasn't
        return 0;
t:// has chinese ch
        return 1;
}

int main(void)
{
	int i = 0;
	char *buf[] = {
		"访客网络",
		"是打发斯蒂芬接口",
		"我是谁",
		"hello",
		"我hi",
		"world我",
	};

	for (i = 0; i < GET_ARRAY_SIZE(buf); i++) {
		if (utf8_chinese_check(buf[i])) {
			DEBUG("str:%s has chinese ch.\n", buf[i]);	
		}	
	}
	
	return 0;
}
