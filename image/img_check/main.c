#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "mem.h"

#define IMG_TYPE_PNG	"PNG"
#define IMG_TYPE_BMP	"BM"
#define IMG_TYPE_JPG	"JFIF"
#define IMG_TYPE_JPEG	IMG_TYPE_JPG	

struct img{
	char *format;	// 文件扩展名，后缀名
	int offset;	
	char *type;	// 文件头中的文件类型名称
};

struct img images[] = {
	{
		.format	= "bmp",
		.offset	= 0,
		.type	= IMG_TYPE_BMP,
	}, 
	{
		.format	= "png",
		.offset	= 1,
		.type	= IMG_TYPE_PNG,
	}, 
	{
		.format	= "jpg",
		.offset	= 6,
		.type	= IMG_TYPE_JPG,
	}, 
	{
		.format	= "jpeg",
		.offset	= 6,
		.type	= IMG_TYPE_JPEG,
	}
};

static char * get_format_by_name(char *name)
{
	int i = 0;

	CHECK_P_VALID(name, NULL);

	for (i = strlen(name) - 2; i > -1; i--) {
		if (name[i] == '.') {
			DEBUG("get format:%s name:%s\n", &name[i+1], name);
			return &name[i+1];
		}
	}

	ERROR("format not found , name:%s\n", name);
	return NULL;
}

static struct img *get_img_by_name(char *const name)
{
	int i = 0;
	char *f = NULL;

	CHECK_P_VALID(name, NULL);

	if ((f = get_format_by_name(name)) == NULL) {
		return NULL;
	}

	for (i = 0; i < GET_ARRAY_SIZE(images); i++) {
		if (!strcmp(f, images[i].format)) 
			return &images[i];
	}
	return NULL;
}

/*
 * 0: valid
 * 1: invalid
 * 2: err
 * */
static int img_head_check(struct img * const i, char * const name) 
{
	FILE *f = NULL;
	char buf[64] = {0};
	int ret;
	int rlen = 0;

	CHECK_P_VALID(i, 2);
	CHECK_P_VALID(name, 2);

	if ((f = fopen(name, "r")) == NULL) {
		ERROR("open %s faild! err:%s\n", name, strerror(errno));
		ret	= 2;
		goto done;
	}

	fseek(f, i->offset, SEEK_SET);
	rlen	= strlen(i->type);
	if (fread(buf, sizeof(char), rlen, f) != rlen) {
		ERROR("read %s faild! err:%s\n", name, strerror(errno));
		ret	= 2;
		goto done;
	}

	if (!memcmp(buf, i->type, rlen)) {
		DEBUG("%s check valid!\n", name);
		ret	= 0;
		goto done;
	} else {
		DEBUG("%s check invalid! type:%s real_type:%s\n", name, i->type, buf);
		ret	= 1;
		goto done;
	}

done:
	if (f)
		fclose(f);

	return ret;
}

/*
 * 0: valid
 * 1: invalid
 * 2: err
 * */
static int img_valid_check(char * const name)
{
	CHECK_P_VALID(name, 2);
	struct img *i = NULL;
	int ret;

	printf("------------checking %s--------------\n", name);

	if ((i = get_img_by_name(name))) {
		ret	= img_head_check(i, name);	
	} else {
		ERROR("unkown image, not support now. name:%s\n", name);
		ret	= 2;
	}

	printf("----------checking done, ret:%d----------\n", ret);
	return ret;
}

int main(int argc, char *argv[])
{
	int i = 1;

	if (argc < 2) {
		DEBUG("Need more param!\n");
		return -1;
	}

	while (argv[i])
		img_valid_check(argv[i++]);	

	return 0;
}
