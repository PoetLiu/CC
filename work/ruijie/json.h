#ifndef __APP_JSON_H_
#define __APP_JSON_H_

#include "cJSON.h"

cJSON * json_load_buffer(char * buf);
cJSON * json_load_file(char * filename);

char * json_dump_buffer(cJSON * json , int fmt);
int json_dump_file(cJSON * json , char * filename, int fmt);

#endif /* __APP_JSON_H_ */
