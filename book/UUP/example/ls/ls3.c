/*
   *
   * Authored by Liu Peng on: Tue Oct 28 15:40:21 CST 2014
   *
   * @desc: ls3.c 
   *	    purpose list contents of directory or directories
   *	    action if no args, use . else list files in args 	
   *	    note usrs stat and pwd.h and grp.h
   * @func: list of file's infomation of a dir/file
   * 相比上一个版本， 修正了不能查看指定目录信息的bug
   * 改变不同文件类型的文件名显示颜色
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define BLUE	"01;34"
#define GREEN	"01;32"
#define WHITE	"40;37"
#define MAX(m, n) ((m)>(n)?(m):(n))
#define MAX_S(m, n) 				\
	do {					\
		if (strlen(n) > strlen(m))	\
			strcpy(m, n);		\
	} while(0)
#define GET_BITS(n, b) 				\
	(b)	= 1;				\
	while ((n)/10){				\
		(n) /= 10;			\
		++(b);				\
	}

typedef unsigned char uchar;
struct print_format {
	uchar links_w;
	uchar user_w;
	uchar group_w;
	uchar fname_w;
	uchar size_w;
};

void do_ls(char []);
void dostat(char *, struct print_format *);
void show_file_info(char *, struct stat *, struct print_format *);
void mode_to_letters(int, char []);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);


int main(int ac, char *av[])
{

	if (ac == 1)
		do_ls(".");
	else { 
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
	}
	return 0;
}

static void get_print_format(struct dirent **nl, int n, 
		struct print_format* pf)
{
	int i;
	nlink_t max_links = 0;
	off_t max_size = 0;
	int max_uname = 0;
	int max_gname = 0;
	int max_fname = 0;
	struct stat info;

	for (i = 0; i < n; i++) {
		if (stat(nl[i]->d_name, &info) == -1)
			perror(nl[i]->d_name);
		max_links	= MAX(max_links, info.st_nlink);
		max_size	= MAX(max_size, info.st_size);
		max_uname	= MAX(max_uname, strlen(uid_to_name(info.st_uid)));
		max_gname	= MAX(max_gname, strlen(gid_to_name(info.st_gid)));
		max_fname	= MAX(max_fname, strlen(nl[i]->d_name));
	}	
/*	printf("max_links:%d\n", max_links);
	printf("max_size:%d\n", max_size);
	printf("max_uname:%d\n", max_uname);
	printf("max_gname:%d\n", max_gname);
	printf("max_fname:%d\n", max_fname);
	*/

	GET_BITS(max_links, pf->links_w);
	GET_BITS(max_size, pf->size_w);
	pf->user_w	= max_uname;
	pf->group_w	= max_gname;
	pf->fname_w	= max_fname;

/*	printf("max_link_w:%d\n", pf->links_w);
	printf("max_size_w:%d\n", pf->size_w);
	*/
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
	struct dirent **namelist;
	struct print_format pf;
	int 	n, i;
	
	if (chdir(dirname) < 0)
		perror("chdir faild");

	n	= scandir(".", &namelist, 0, alphasort);
	if (n < 0)
		perror("not found\n");
	else {	
		get_print_format(namelist, n, &pf);
		for(i = 0; i < n; i++) {
			dostat(namelist[i]->d_name, &pf);
			free(namelist[i]);
		}
		free(namelist);
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
void dostat(char *filename, struct print_format * pf)
{
	struct stat info;

	if (stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info, pf);
}

static void get_filename_color(char *modestr, char **color)
{
	if (modestr[0] == 'd')
		*color	= BLUE;
	else if (modestr[3] == 'x')
		*color	= GREEN;
	else 
		*color	= WHITE;
}
/*
   * Funcion	: display the info about filename. The info is
   *		  stored in struct at *info_p
   * Author	:
   *
   * arguments	:
   * return	:
   */
void show_file_info(char *filename, struct stat *info_p, struct print_format * pf)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	char *color;
	void mode_to_letters();
	char modestr[11];
	char print_str[128];

	mode_to_letters(info_p->st_mode, modestr);

	printf("%s ", modestr);
	printf("%*d ", pf->links_w, (int)info_p->st_nlink);
	printf("%-*s ", pf->user_w, uid_to_name(info_p->st_uid));
	printf("%-*s ", pf->group_w, gid_to_name(info_p->st_gid));
	printf("%*ld ", pf->size_w, (long)info_p->st_size);
	printf("%.12s ", 4 + ctime(&info_p->st_mtime));
	get_filename_color(modestr, &color);
	sprintf(print_str, "\033[%sm%s\033[0m\n", color, filename);
	printf(print_str);
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
