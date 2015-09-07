/*
   *
   * Authored by Liu Peng on: Tue Oct 28 15:40:21 CST 2014
   *
   * @desc: ls2.c 
   *	    purpose list contents of directory or directories
   *	    action if no args, use . else list files in args 	
   *	    note usrs stat and pwd.h and grp.h
   * @func: list of file's infomation of a dir/file
   * @bug: try ls2 /tmp
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

void do_ls(char []);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);

int main(int ac, char *av[])
{

	if (ac == 1)
		do_ls(".");
	else { 
		while (--ac) {
			printf("%s\n", *++av);
			do_ls(*av);
		}
	}
	return 0;
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
		fprintf(stderr, "ls2:cannot open %s\n", dirname);
	else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
				continue;
			dostat(direntp->d_name);
		}
		closedir(dir_ptr);
	}
}

/*
   * Funcion	: 
   *
   * Author	:
   *
   * arguments	:
   * return	:
   */
void dostat(char *filename)
{
	struct stat info;

	if (stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

/*
   * Funcion	: display the info about filename. The info is
   *		  stored in struct at *info_p
   * Author	:
   *
   * arguments	:
   * return	:
   */
void show_file_info(char *filename, struct stat *info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];

	mode_to_letters(info_p->st_mode, modestr);

	printf("%s", modestr);
	printf("%2d ", (int)info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%8ld ", (long)info_p->st_size);
	printf("%.12s ", 4 + ctime(&info_p->st_mtime));
	printf("%s\n", filename);
}

/*
 * utility functons
 */

/*
 * This function takes a mode value and a char array
 * and puts into the char array the file type and the
 * nine letters that correspond to the bits in mode.
 * NOTE: It does not code setuid, setgid, and sticky
 * codes
 */
void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");		/* default=no perms */

	if (S_ISDIR(mode)) str[0] = 'd';	/* directory? */
	if (S_ISCHR(mode)) str[0] = 'c';	/* char devices */
	if (S_ISBLK(mode)) str[0] = 'b';	/* block devices */
	
	if (mode & S_IRUSR) str[1] = 'r';	/* 3bits for user */
	if (mode & S_IWUSR) str[2] = 'w';	
	if (mode & S_IXUSR) str[3] = 'x';

	if (mode & S_IRGRP) str[4] = 'r';	/* 3bits for group */
	if (mode & S_IWGRP) str[5] = 'w';	
	if (mode & S_IXGRP) str[6] = 'x';
	
	if (mode & S_IROTH) str[7] = 'r';	/* 3bits for other */
	if (mode & S_IWOTH) str[8] = 'w';	
	if (mode & S_IXOTH) str[9] = 'x';
}

#include <pwd.h>
/*
   * Funcion	: returns pointer to username associated with uid, 
   *		  use getpw()
   * Author	:
   *
   * arguments	:
   * return	:
   */
char *uid_to_name(uid_t uid)
{
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL) {
		sprintf(numstr, "%d", uid);
		return numstr;
	} else 
		return pw_ptr->pw_name;
}

#include <grp.h>
/*
   * Funcion	: returns pointers to group member gid. 
   * 		  used getgrgid(3)
   * Author	:
   *
   * arguments	:
   * return	:
   */
char * gid_to_name(gid_t gid)
{
	struct group *getgrgid(), *grp_ptr;
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL) {
		sprintf(numstr, "%d", gid);
		return numstr;
	} else
		return grp_ptr->gr_name;
}
