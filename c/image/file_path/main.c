/*
 * Remove path, whitch have '\' in input
 * Exp, input:c:\hello\test.c
 * 	output:test.c
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "mem.h"

int main(int argc, char *argv[])
{
	int i = 1, len = 0;
	char tmp[1024] = {0}, *name_start = argv[1], *p = NULL;

	if (argc < 2) {
		DEBUG("Need more param!\n");
		return -1;
	}

	if ((p = strrchr(argv[1], '\\')) != NULL) {
		name_start	= p;
	}

	len	= strlen(name_start);
	memcpy(tmp, name_start, len);
	DEBUG("name:%s\n", tmp);
	return 0;
}
