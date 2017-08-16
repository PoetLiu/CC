#include<stdio.h>

#define MSG_STR_DEF(x)	[(x)]=#x
enum WPS_SET_E
{
	WPS_SET_MODE_PIN_VENDOR=0,
	WPS_SET_MODE_PIN_ENROLLEE,
	WPS_SET_MODE_PBC,
};

char *type_str[] = 
{
	MSG_STR_DEF(WPS_SET_MODE_PIN_VENDOR),
        MSG_STR_DEF(WPS_SET_MODE_PIN_ENROLLEE),
        MSG_STR_DEF(WPS_SET_MODE_PBC),
	[(0)]="HELLO,WOLRD",
};

int main(void)
{
	int i;

	for(i = 0; i < sizeof(type_str)/sizeof(char *); i++)
		printf("第%d个模式：%s\n", i, type_str[i]);

	return 0;
}
