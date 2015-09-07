/*
   *
   * Authored by Liu Peng on: Tue Oct 28 15:40:21 CST 2014
   *
   * @desc: ls.c v4
   *	    purpose list contents of directory or directories
   *	    action if no args, use . else list files in args 	
   *	    note usrs stat and pwd.h and grp.h
   * @func: list of file's infomation of a dir/file
   * 相比上一个版本, 修正了如下bug:
   * 1. 不加-l 时， 一般模式输出增加颜色显示， 调整程序流程、架构
   * 2. 不加-l 时， 分组显示偶尔出现的少显示一行的bug
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define MAX_PATH	1024
#define BLUE		"\033[01;34m"
#define GREEN		"\033[01;32m"
#define GREEN_BACK_BLUE	"\033[42;34m"
#define WHITE_LIGHT	"\033[01;37m"
#define WHITE		"\033[40;37m"
#define PROG_NAME	"myls"

#define MAX(m, n) ((m)>(n)?(m):(n))
#define GET_BITS(n, b) 				\
	(b)	= 1;				\
	while ((n)/10){				\
		(n) /= 10;			\
		++(b);				\
	}

int long_format = 0;
int print_all	= 0;

typedef unsigned char uchar;
struct print_format {
	uchar	links_w;
	uchar	user_w;
	uchar	group_w;
	uchar	fname_w;
	uchar	size_w;

	uchar	**fname;
	uchar	*col_w;
	int 	cols;
	int 	lines;
	int	all_nums;
};

void do_ls(char []);
void dostat(uchar *, uchar , struct print_format *);
void show_file_info(char *, uchar , struct stat *, struct print_format *);
void mode_to_letters(int, char []);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);
static void usage();
void mode_to_letters(int mode, char str[]);
static char * get_filename_color(char *modestr);

int main(int ac, char *av[])
{

	int 	i, opt;

	while ((opt = getopt(ac, av, "al")) != -1) {
		switch (opt) {
			case 'a':
				print_all	= 1;
				break;
			case 'l':
				long_format	= 1;
				break;
			case '?':
				usage();
				exit(1);
				break;
			default:
				;
		}
	}

	av	+= optind;
	ac	-= optind;
	i	= ac;

	if (*av == NULL)
		do_ls(".");
	else { 
		while (i--) {
			if (i + 1 != ac)
				printf("\n");
			if (ac > 1) 
				printf("%s:\n", *av);
	
			do_ls(*av);
			av++;
		}
	}
	return 0;
}

static void usage()
{
	printf("usage:%s <option> dir\noptions:\n\t-a:print"
		"all items\n\t-l:long formart output\n", PROG_NAME);
}

static void get_win_col(int *col)
{
	struct winsize ws;
	if (ioctl(0, TIOCGWINSZ, &ws) != 0)
		perror("ioctl error");
	*col	= ws.ws_col;
}


static void get_print_format(struct dirent **nl, int n, 
		struct print_format* pf)
{
	int i, j = 0, ws_col, lines = 0, cols, col_mw, col_tw;
	nlink_t max_links = 0;
	off_t max_size = 0;
	int max_uname = 0;
	int max_gname = 0;
	int max_fname = 0;
	struct stat info;
	uint32_t total = 0;
	uchar	*fnames_l, **tmp_name;
	
	tmp_name	= (uchar **) malloc(sizeof(uchar *) * n);
	get_win_col(&ws_col);
	if (!long_format) {
		pf->col_w	= (uchar *) malloc(n);
		fnames_l	= (uchar *) malloc(n);
		if (pf->col_w == NULL || fnames_l == NULL) {
			perror("malloc error");
			exit(1);
		}
		for (i = 0; i < n; i++) {
			if (nl[i]->d_name[0] == '.' && !print_all)
				continue;
			tmp_name[j] = (uchar *) malloc(strlen(nl[i]->d_name) + 1);
			fnames_l[j] = strlen(nl[i]->d_name);
			strcpy(tmp_name[j], nl[i]->d_name);
			j++;
		}
		n	= j;
		// 非长格式输出， 需要计算合适的行数和列数
		while(1) {
			lines++;
			cols	= n%lines ? n/lines + 1 : n/lines;
			for (i = 0, col_tw = 0; i < cols; i++) {
				for (j = 0, col_mw = 0; j < lines && i*lines + j < n; j++) 
					col_mw	= MAX(col_mw, fnames_l[i*lines + j]);
				pf->col_w[i]	= col_mw;
				col_tw		= col_tw + col_mw + 2;
			}
			if (col_tw - 1 <= ws_col) {
				free(fnames_l);	
				pf->cols	= cols;
				pf->lines	= lines;
				pf->all_nums	= n;
				pf->fname	= tmp_name;
				return;
			}
		}		
	} else {
		for (i = 0; i < n; i++) {
			if (nl[i]->d_name[0] == '.' && !print_all)
				continue;
			
			tmp_name[j] = (uchar *) malloc(strlen(nl[i]->d_name) + 1);
			strcpy(tmp_name[j], nl[i]->d_name);
			j++;
			if (lstat(nl[i]->d_name, &info) == -1)
				perror(nl[i]->d_name);
	
			max_links	= MAX(max_links, info.st_nlink);
			max_size	= MAX(max_size, info.st_size);
			max_uname	= MAX(max_uname, strlen(uid_to_name(info.st_uid)));
			max_gname	= MAX(max_gname, strlen(gid_to_name(info.st_gid)));
			max_fname	= MAX(max_fname, strlen(nl[i]->d_name));
			total		+= info.st_blocks;
		}
	
		pf->fname	= tmp_name;
		GET_BITS(max_links, pf->links_w);
		GET_BITS(max_size, pf->size_w);
		pf->user_w	= max_uname;
		pf->group_w	= max_gname;
		pf->fname_w	= max_fname;
		pf->all_nums	= j;
		printf("total:%d\n", total/2);
		return;
	}

}

/*
 * Funcion	: list files in directory called dirname
 */
void do_ls(char dirname[])
{
	struct dirent **namelist;
	struct print_format pf;
	char path[MAX_PATH];
	int 	n, i;
	
	getcwd(path, MAX_PATH);
	if (chdir(dirname) < 0)
		perror("chdir faild");

	n	= scandir(".", &namelist, 0, alphasort);
	if (n < 0)
		perror("not found\n");
	else {
		get_print_format(namelist, n, &pf);

		for(i = 0; i < n; i++)
			free(namelist[i]);
		free(namelist);

		if (!long_format) {
			int j;		
			for (i = 0; i < pf.lines; i++) {
				for (j = 0; j < pf.cols; j++) {
					if (j*pf.lines + i < pf.all_nums) {
						dostat(pf.fname[j*pf.lines + i], (uchar)j, &pf);
						free(pf.fname[j*pf.lines + i]);
					}
				}
				printf("\n");
			}
			free(pf.col_w);
		} else {
			for(i = 0; i < pf.all_nums; i++) {
				dostat(pf.fname[i], i, &pf);
				free(pf.fname[i]);
			}
		}
		free(pf.fname);
	}
	
	if (chdir(path) < 0)
		perror("chdir faild");
	
}

/*
 * Funcion	: 
 */
void dostat(uchar *filename, uchar pos, struct print_format * pf)
{
	struct stat info;

	if (lstat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, pos, &info, pf);
}

static int is_all_set(char *ms)
{
	int i;

	for ( i = 0; ms[i] != '\0'; i++) {
		if (ms[i] == '-')
			break;
	}

	if (ms[i] == '\0')
		return 1;
	else 
		return 0;
}

static char * get_filename_color(char *modestr)
{
	char *color;
	if (modestr[0] == 'd') {
		if (is_all_set(modestr))
			color	= GREEN_BACK_BLUE;
		else
			color	= BLUE;
	} else if (modestr[0] == 'l') {
		color	= WHITE_LIGHT;
	} else if (modestr[3] == 'x')
		color	= GREEN;
	else 
		color	= WHITE;
	return color;
}

static int is_end_of_line(uchar pos, uchar cols)
{
	return (pos + 1)%cols ? 0 : 1;
}

/*
 * Funcion	: display the info about filename. The info is
 *		  stored in struct at *info_p
 */
void show_file_info(char *filename, uchar pos, struct stat *info_p, struct print_format * pf)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	char link_target[MAX_PATH];
	int len;

	mode_to_letters(info_p->st_mode, modestr);

	if (!long_format) {
		if (is_end_of_line(pos, pf->cols)) 
			printf("%s%-*s\033[0m ", get_filename_color(modestr), pf->col_w[pos], filename);
		else
			printf("%s%-*s\033[0m  ", get_filename_color(modestr), pf->col_w[pos], filename);
	} else {
		printf("%s ", modestr);
		printf("%*d ", pf->links_w, (int)info_p->st_nlink);
		printf("%-*s ", pf->user_w, uid_to_name(info_p->st_uid));
		printf("%-*s ", pf->group_w, gid_to_name(info_p->st_gid));
		printf("%*ld ", pf->size_w, (long)info_p->st_size);
		printf("%.12s ", 4 + ctime(&info_p->st_mtime));
		printf("%s%s\033[0m", get_filename_color(modestr), filename);
		
		if (modestr[0] == 'l') {
			if ((len = readlink(filename, link_target, 128)) == -1) {
				perror("readlink");
			}else {
				link_target[len] = '\0';
				printf(" -> %s", link_target);
			}
		}
		printf("\n");
	}
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
	if (S_ISLNK(mode)) str[0] = 'l';
	
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
