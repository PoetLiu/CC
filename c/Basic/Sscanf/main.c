#include <stdio.h>

int main(void)
{
	char *p	= "192.168.2.1     	www.baidu.com\n";
	char ip[50], url[50];

	sscanf(p, "%s%s", ip, url);
	printf("ip:%s\nurl:%s\n", ip, url);
	return 0;
}
