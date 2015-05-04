#include<stdio.h>

/*
 * size of mac:12, size of mac[0]:6, size of mac[0][0]:1
 * Mac 00:1C:C091:98:68
 * size_char_array:4
 * size_int_array:4
 *
 */

int size_char_array(char mac[][6])
{
	return sizeof(mac);
}

int size_int_array(int test[])
{
	return sizeof(test);
}

int main(void)
{
	int i, j;
	char mac[2][6] = {
		0x30, 0x30, 0x31, 0x43, 0x43, 0x30,
		0x39, 0x31, 0x39, 0x38, 0x36, 0x38
	};

	int test[10];

	printf("size of mac:%d, size of mac[0]:%d, size of mac[0][0]:%d\n", \
			sizeof(mac), sizeof(mac[0]), sizeof(mac[0][0]));

	printf("Mac ");
	for(i = 0; i < sizeof(mac)/sizeof(mac[0]); i++){
		for(j = 0; j < sizeof(mac[0]); j++){
			printf("%c", mac[i][j]);
			if( j%2	== 1 && j != sizeof(mac[0]) - 1)
				printf(":");
		}
	}
	printf("\n");

	printf("size_char_array:%d\n", size_char_array(mac));
	printf("size_int_array:%d\n", size_int_array(test));
	return 0;
}
