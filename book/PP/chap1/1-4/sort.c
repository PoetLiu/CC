/*
 * Authored by Liu Peng on: Mon Sep  7 13:43:53 CST 2015
 * @desc: Bitmap sort
 * @func:
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "debug.h"
#include "mem.h"
#include "./data/data.h"

typedef uint8_t bitmap_t;

enum SORT_TYPE {
	SORT_TYPE_ASC,
	SORT_TYPE_DESC,
};

#define SHIFT	3
#define MASK	0x7	
static inline void bit_set(bitmap_t *map, int i)
{ 
	map[i>>SHIFT]	|= (1 << (i & MASK)); 
}

static inline void bit_clr(bitmap_t *map, int i)
{ 
	map[i>>SHIFT]	&= ~(1 << (i & MASK)); 
}

static inline int bit_test(bitmap_t *map, int i)
{ 
	return map[i>>SHIFT] & (1 << (i & MASK)); 
}

static bitmap_t * bitmap_init(int size)
{
	uint8_t *map = NULL;
	
	if (size <= 0) {
		DEBUG("invalid value of size:%d\n", size);
		return NULL;
	}
	map	= calloc(size, sizeof(bitmap_t));
	P_VALID_CHECK_RET(map, NULL);

	return map;	
}

static int bitmap_set(bitmap_t *map, int bit_num)
{
	P_VALID_CHECK_RET(map, -1);
	if (bit_num > DATA_MAX || bit_num < 0) {
		DEBUG("invalid bit vaule:%d\n", bit_num);	
		return -1;
	}
	bit_set(map, bit_num);

	return 0;
}

static int bitmap_is_set(bitmap_t *map, int bit_num)
{
	P_VALID_CHECK_RET(map, -1);
	if (bit_num > DATA_MAX || bit_num < 0) {
		DEBUG("invalid bit vaule:%d\n", bit_num);	
		return -1;
	}
	return bit_test(map, bit_num);
}

static int bitmap_create(char *path, bitmap_t *map)
{
	int i = 0;
	FILE *fp = NULL;
	int data = 0, err = 0;

	P_VALID_CHECK_RET(path, -1);
	P_VALID_CHECK_RET(map, -1);
	fp	= fopen(path, "r");
	P_VALID_CHECK_RET(fp, -1);

	for (i = 0; i < DATA_NUM; i++) {
		if (fscanf(fp, "%d", &data) != 1) {
			DEBUG("read data faild:%s\n", strerror(errno));
			err	= -1;
			goto ret;
		}
		bitmap_set(map, data);		
	}

ret:
	if (fp)
		fclose(fp);
	fp	= NULL;
	return err;
}

/*
 * Funcion	: sort and print
**/
static int bitmap_print(char *path, int type, bitmap_t *map)
{
	FILE *fp = NULL;
	int i = 0, err = 0;

	P_VALID_CHECK_RET(path, -1);
	P_VALID_CHECK_RET(map, -1);
	fp	= fopen(path, "w+");
	P_VALID_CHECK_RET(fp, -1);

	switch (type) {
		case SORT_TYPE_DESC:
			for (i = DATA_NUM - 1; i > -1; i--)
				if (bitmap_is_set(map, i))
					fprintf(fp, "%d\n", i);
			break;
		case SORT_TYPE_ASC:
			for (i = 0; i < DATA_NUM; i++)
				if (bitmap_is_set(map, i))
					fprintf(fp, "%d\n", i);
			break;
		default:
			DEBUG("unkown sort type:%d\n", type);
			err	= -1;
			break;

	}

	if (fp)
		fclose(fp);
	fp	= NULL;
	return err;
}

static int bitmap_destroy(bitmap_t **map)
{
	P_VALID_CHECK_RET(map, -1);
	SAFE_FREE(*map);
	return 0;
}

int main(void)
{
	bitmap_t *map = NULL;
	map	= bitmap_init(BITMAP_SIZE);
	P_VALID_CHECK_RET(map, -1);
	bitmap_create(DATA_IN, map);
	bitmap_print(DATA_OUT, SORT_TYPE_DESC, map);
	bitmap_destroy(&map);

	return 0;
}
