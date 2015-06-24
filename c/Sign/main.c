#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef uchar * byte_pointer;

void show_bytes(byte_pointer addr, int size)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		printf("%02x", addr[i]);
		if (i != size - 1)
			printf(" ");
	}	
	printf("\n");
}

int item_counter;
struct test_item {			
	char *name;	
	char buf[4];
	int len;	
	byte_pointer p;	
};								

struct test_item *menu[100];

#define ITEM_ADD(f, t, d) 							\
	do { 									\
		struct test_item *p;						\
		p	= (struct test_item*)malloc(sizeof(struct test_item));	\
		p->name 	= #f"("#d") to "#t;				\
		p->len 		= sizeof(t);					\
		*(t*)p->buf	= (f)d;						\
		menu[item_counter++]	= p;					\
	}while(0)


int main(void)
{

	ITEM_ADD(char, int, 0x5f);
	ITEM_ADD(char, int, 0xff);

	ITEM_ADD(char, uint, 0x5f);
	ITEM_ADD(char, uint, 0xff);

	ITEM_ADD(uchar, int, 0x5f);
	ITEM_ADD(uchar, int, 0xff);

	ITEM_ADD(uchar, uint, 0x5f);
	ITEM_ADD(uchar, uint, 0xff);

	ITEM_ADD(int, char, 0x5f);
	ITEM_ADD(int, char, 0xffffffffu);

	ITEM_ADD(int, uchar, 0x5f);
	ITEM_ADD(int, uchar, 0xfffffffu);

	ITEM_ADD(uint, char, 0x5f);
	ITEM_ADD(uint, char, 0xfffffffu);

	ITEM_ADD(uint, uchar, 0x5f);
	ITEM_ADD(uint, uchar, 0xffffffu);

	int j;

	for (j = 0; j < item_counter; j++) {
		printf("%-25s\t", menu[j]->name);
		show_bytes((byte_pointer)menu[j]->buf, menu[j]->len);
		free(menu[j]);
	}

	return 0;
}
