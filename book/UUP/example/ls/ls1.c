/*
   *
   * Authored by Liu Peng on: Tue Oct 28 14:40:32 CST 2014
   *
   * @desc: ls1.c purpose list contents of directory or directories
   *
   * @func: action if no args, use . else list files in args
   */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char []);

int main(int ac, char *av[])
{
	if (ac ==1)
		do_ls(".");
	else
		while(--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
}

/*
   * Funcion	: list files in directory called dirname
   *
   * Author	:
   *
   * arguments	:
   * return	:
   */
void do_ls(char dirname[])
{

	DIR *dir_ptr;
	struct dirent *direntp;

	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else{
		while ((direntp = readdir(dir_ptr)) != NULL)
			printf("%s\n", direntp->d_name);
		closedir(dir_ptr);
	}
}
