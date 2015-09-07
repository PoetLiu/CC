/*
 *
 * Authored by Liu Peng on: Tue Nov  4 11:25:30 CST 2014
 *
 * @desc: shows how to use popen to write to a process that
 *	  reads from stdin. This program writes email to two
 *	  users. Note how easy it is to use fprintf to format
 * 	  the data to send.
 *
 * @func:
**/

#include <stdio.h>

int main(void)
{
	FILE	*fp;

	fp	= popen("mail liupeng backup", "w");
	fprintf(fp, "Error with backup!!\n");
	pclose(fp);
}
