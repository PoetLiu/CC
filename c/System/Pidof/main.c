#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

int main(void)
{
	FILE *fp = NULL;
	char buf[1024] = {0};
	pid_t pid;

	if ((fp = popen("pidof init", "r")) != NULL) {
		if (fgets(buf, sizeof(buf), fp)) {
			DEBUG("get pipe data:%s\n", buf);			
			sscanf(buf, "%d", &pid);
			DEBUG("get pid:%d\n", pid);			
			goto exit;	
		}
	
	} else {
		DEBUG("open pipe faild\n");
		return -1;
	}

exit:
	if (fp)
		pclose(fp);
	return 0;
}
