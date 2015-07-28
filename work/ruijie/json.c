#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"


#define NAS_DEBUG(fmt, args...) do{if(1)console_printf("DEBUG:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)
#define NAS_ERROR(fmt, args...) do{if(1)console_printf("ERROR:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)

cJSON * json_load_buffer(char * buf)
{
	cJSON * json;

	NAS_DEBUG("buf=%s\n", buf);
	json = cJSON_Parse(buf);
	if (!json) 
		NAS_ERROR("%s\n", cJSON_GetErrorPtr());

	return json;
}

cJSON * json_load_file(char * filename)
{
	cJSON * json;
	FILE * f;
	char * buf;
	struct stat s;

	if ( -1 == stat(filename, &s)) {
		NAS_ERROR("stat error, %s", filename);
		return NULL;
	}

	buf = (char *)malloc(s.st_size + 1);
	if (!buf) {
		NAS_ERROR("alloc failed\n");
		return NULL;
	}

	memset(buf, 0, s.st_size + 1);

	f = fopen(filename, "r");
	if (!f) {
		NAS_ERROR("open file %s failed\n", filename);
		free(buf);
		return NULL;
	}

	fread(buf, s.st_size, 1, f);
	NAS_DEBUG("buf=%s\n", buf);
	json = cJSON_Parse(buf);
	NAS_DEBUG("\n");
	if (!json) 
		NAS_ERROR("%s\n", cJSON_GetErrorPtr());

	fclose(f);
	free(buf);
	return json;
}

char * json_dump_buffer(cJSON * json, int fmt)
{
	char * out;

	if (fmt)
		out = cJSON_Print(json);
	else
		out = cJSON_PrintUnformatted(json);
	NAS_DEBUG("out=%s\n",out);

	return out;
}

int json_dump_file(cJSON * json , char * filename, int fmt)
{
	FILE * f;
	char * out;

	f = fopen(filename, "w");
	if (!f) {
		NAS_ERROR("open file %s failed", filename);
		return -1;
	}

	if (fmt)
		out = cJSON_Print(json);
	else
		out = cJSON_PrintUnformatted(json);
	NAS_DEBUG("out=%s\n",out);
	fputs(out, f);
	free(out);

	fclose(f);
	return 0;
}
