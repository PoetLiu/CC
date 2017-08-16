/*
   *
   * Authored by Liu Peng on: Fri Aug 15 17:53:25 CST 2014
   *
   * @desc: 打开一个文件，并清空内容，保持文件最后修改时间不变
   *
   * @func: 验证utime对最后修改时间的设置
   */
#include <stdio.h>
#include <stdlib.h>
#include <utime.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char * argv[])
{
	int i, fd;
	struct stat	statbuf;
	struct utimbuf	timebuf;

	for (i = 1; i < argc; i++) {
		if (stat(argv[i], &statbuf) < 0) {
			printf("%s: stat error : %s\n", argv[i], strerror(errno));
			continue;
		}

		if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0)  {
			printf("%s: open error :%s\n", argv[i], strerror(errno));
			continue;
		}

		close(fd);
		timebuf.actime	= statbuf.st_atime;
		timebuf.modtime	= statbuf.st_mtime;
		if (utime(argv[i], &timebuf) < 0) {
			printf("%s: utime error :%s\n", argv[i], strerror(errno));
			continue;
		}
	}
	exit(0);
}
