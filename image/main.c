#include <stdio.h>
#include "debug.h"
#include "mem.h"

#define IMG_TYPE_PNG	"PNG"
#define IMG_TYPE_BMP	"BM"
#define IMG_TYPE_JPG	"JFIF"
#define IMG_TYPE_JPEG	IMG_TYPE_JPG	

struct img{
	char *ext_fn;	// 文件扩展名，后缀名
	int offset;	
	char *fh_fn;	// 文件头中的文件类型名称
};

struct images[] = {
	{
		.ext_fn	= "bmp",
		.offset	= 0,
		.fh_fn	= IMG_TYPE_BMP,
	}, 
	{
		.ext_fn	= "png",
		.offset	= 1,
		.fh_fn	= IMG_TYPE_PNG,
	}, 
	{
		.ext_fn	= "jpg",
		.offset	= 6,
		.fh_fn	= IMG_TYPE_JPG,
	}, 
	{
		.ext_fn	= "jpeg",
		.offset	= 6,
		.fh_fn	= IMG_TYPE_JPEG,
	}
};

struct img *get_img_by_name(char *const name)
{
	int i = 0;
	char *p = NULL;

	if (!name) {
		DEBUG("null name:%s\n", p);
		return NULL;
	}

	if ((p = strstr(name, "."))) {
		p	+= strlen(".");	
		DEBUG("get fname:%s\n", p);
	} else {
		DEBUG("cann't get fname\n");
	}

	for (i = 0; i < GET_ARRAY_SIZE(images); i++) {
		if (strcmp(p, images[i].ext_fn))
	}
	return NULL;
}

int main(void)
{

	return 0;
}
