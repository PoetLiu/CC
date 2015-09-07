/*
   *
   * Authored by Liu Peng on: Wed Oct 29 09:53:00 CST 2014
   *
   * @desc: spwd.c a simplified version of pwd
   *
   * @func: starts in current directory and recursively
   *	    climbs up to root of filesystem, prints top part
   *	    then prints current part
   * uses readdir() to get info about each thing
   * bug: prints an empty string if run from '/'
   */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

ino_t	get_inode(char *);
void	printpathto(ino_t);
void	inum_to_name(ino_t, char *, int);

int main(void)
{
	printpathto(get_inode("."));
	putchar('\n');
	return 0;
}

/*
   * Funcion	: print path leading down to an object with this inode
   *		  kindof recursive
   * Author	:
   *
   * arguments	:
   * return	:
   */
void printpathto(ino_t this_inode)
{
	ino_t my_inode;
	char its_name[BUFSIZ];

	if (get_inode("..") != this_inode) {
		chdir("..");
		inum_to_name(this_inode, its_name, BUFSIZ);
		my_inode	= get_inode(".");
		printpathto(my_inode);
		printf("/%s", its_name);
	}
}

/*
   * Funcion	: looks through current directory for a file with this 
   *		  inode, number and copies its name into namebuf
   * Author	:
   *
   * arguments	:
   * return	:
   */
void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;

	dir_ptr	= opendir(".");
	if (dir_ptr == NULL) {
		perror(".");
		exit(1);
	}

	// search directory for a file with specified inum
	while ((direntp = readdir(dir_ptr)) != NULL) {
		printf("%s:%d\n", direntp->d_name, direntp->d_ino);
		if (direntp->d_ino == inode_to_find) {
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "error looking for inum %d\n", inode_to_find);
	exit(1);
}

/*
   * Funcion	: returns inode number of the file
   *
   * Author	:
   *
   * arguments	:
   * return	:
   */
ino_t get_inode(char * fname)
{
	struct stat info;

	if (stat(fname, &info) == -1) {
		fprintf(stderr, "cannot stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
