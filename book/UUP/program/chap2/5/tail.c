/*
 *
 * Authored by Liu Peng on: Mon Dec  1 17:11:32 CST 2014
 * tail.c
 * @desc: tail的简单实现
 * 	  此版本暂未支持除了文件名之外的参数
 *
 * @func:
**/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEFAULT_OUTPUT_LINES	10
#define BUFFERSIZE		4
#define MIN(m, n)		((m) > (n) ? (n) : (m))
#define my_perror(fmt, args...) 						\
	do { printf(fmt" line:%d""%s\n", ##args, __LINE__, strerror(errno));	\
	}while(0)

// 计算buf中'\n'的数目， 得到当前文件偏移量到文件末尾之间包含的行数
// 当行数等于要求的行数时， 返回该相对偏移量
static int find_print_pos(char *buf, int default_lines) 
{
	// 注意这里必须为static
	static int	lines = 0;
	int	i = strlen(buf);

	// 逆序遍历
	while (i > -1) {
		if (buf[i] == '\n')
			lines++;
		if (lines == default_lines + 1)
			return i + 1;
		i--;
	}
	return 0;
}

// 输出从当前位置开始到文件末尾的内容
static void print_data(int fd, char *buf)
{
	int rlen = 0;

	while ((rlen = read(fd, buf, BUFFERSIZE))) {
		buf[rlen]	= '\0';
		printf("%s", buf);
	}	
}

// 从fd中读len个字节,然后回滚文件位置指针
static int read_and_roll_back(int fd, char *buf, int len)
{
	int rlen = 0;	
	if ((rlen = read(fd, buf, len)) == -1) {
		my_perror("error read");
		return -1;
	}

	buf[rlen]	= '\0';
	if (lseek(fd, -rlen, SEEK_CUR) == -1) {
		my_perror("error lseek");
		return -1;
	}
	return rlen;

}

// 检查是否已经达到了可以打印输出的位置
// 意味着从该位置到文件末尾包含了需要打印的行数
static int is_print_pos(int fd, char *buf, int default_lines)
{
	int print_pos;

	print_pos = find_print_pos(buf, default_lines);
	if (print_pos) {
		if (lseek(fd, print_pos, SEEK_CUR) == -1) {
			my_perror("error lseek");
		}
		return 1;
	}
	return 0;
}

int main(int ac, char *av[]) 
{
	int	fd, pos, rlen, left;
	struct stat fs;
	char 	buf[BUFFERSIZE + 1];

	if (ac < 2) {
		printf("usage %s <option> <file>\n", av[0]);
		return -1;
	}

	fd	= open(av[1], O_RDONLY);
	if (fd == -1) {
		my_perror("error open file");
		return -1;
	}

	if (fstat(fd, &fs) == -1) {
		my_perror("error fstat file");
		return -1;
	}

	pos	= MIN(fs.st_size, BUFFERSIZE);
	if (lseek(fd, -pos, SEEK_END) == -1) {
		my_perror("error lseek pos %d", -pos);
		return -1;
	}

	left	= fs.st_size - pos;

	// 循环直到确定了文件位置
	// 从文件末尾开始，向前读取数据，统计行数, 确定位置
	while (1) {
		if ((rlen = read_and_roll_back(fd, buf, pos)) == -1) {
			my_perror("error read_and_roll_back");
			return -1;
		}

		if (is_print_pos(fd, buf, DEFAULT_OUTPUT_LINES) || left <= 0)
			break;

		pos	= MIN(left, BUFFERSIZE);
		left	-=  rlen;
		if (lseek(fd, -pos, SEEK_CUR) == -1) {
			my_perror("error lseek pos %d", -pos);
			return -1;
		}
	}
	
	print_data(fd, buf);
	close(fd);

	return 0;
}
