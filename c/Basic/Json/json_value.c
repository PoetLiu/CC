#include <stdio.h>
#include <string.h>

char buf[1024];
char urls[4][20]	= {
	"www.hello.com",
	"www.shit.com",
	"www.halt.com",
	"www.waht.com"
};

#define json_value(buf,key,fmt,args...) 					\
	do{ 									\
                snprintf((buf), 1024,"\""key"\":\""fmt"\",", ##args);	\ 
        }while(0)

int main(void)
{
	int i;

	for (i = 0; i < 4; i++) {
		json_value(buf + strlen(buf), "url_%d", "%s", i, urls[i]);
	}


	printf("%s\n", buf);
	return 0;


}
