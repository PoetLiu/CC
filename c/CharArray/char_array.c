#include<stdio.h>

/*
 * size of mac:12, size of mac[0]:6, size of mac[0][0]:1
 * Mac 00:1C:C0:91:98:68
 * size_char_array:4
 * size_int_array:4
 *
 */

int size_char_array(char mac[][6])
{
	printf("mac:%p mac+1:%p\n", mac, mac+1);
	return sizeof(mac);
}

int size_int_array(int test[])
{
	return sizeof(test);
}

int main(void)
{
	int i, j;
	char mac[][6] = {
		0x30, 0x30, 0x31, 0x43, 0x43, 0x30,
		0x39, 0x31, 0x39, 0x38, 0x36, 0x38
	};

	unsigned char mac1[6] = {
		0x00, 0x1C, 0xC0, 0x91, 0x98, 0x98
	};

	printf("mac1: %x-%x-%x-%x-%x-%x\n", mac1[0], mac1[1], mac1[2], mac1[3], mac1[4], mac1[5]);

	char mac2[6] = {
		0x00, 0x1C, 0xC0, 0x91, 0x98, 0x98
	};
	printf("mac2: %x-%x-%x-%x-%x-%x\n", mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5]);

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
