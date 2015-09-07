/*
 *
 * Authored by Liu Peng on: Wed Nov  5 09:58:17 CST 2014
 *
 * @desc	: a minimal web server(version 0.2)
 * @usage	: ws portnumber
 * @features	: supports the GET command only runs in the current
 *		  directory forks a new child to handle each request
 *		  has MAJOR security holes, for demo purposes only has
 *		  many other weakness, but is a good start 
 * @build	: cc webserv.c socklib.c -o webserv
 *
 * @func:
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
 * Funcion	: skip over all request info until a CRNL is seen
 * A chorme http request header:
   GET / HTTP/1.1
   Host: 192.168.1.4:12345
   Connection: keep-alive
   Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,* / *;q=0.8
   User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.103 Safari/537.36
   Accept-Encoding: gzip,deflate,sdch
   Accept-Language: zh-CN,zh;q=0.8,en;q=0.6
 * we just get the first line "GET / HTTP/1.1" and stip over other message
*/
void read_til_crnl(FILE *fp)
{
	char	buf[BUFSIZ];
	while (fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0);
}


/*
 * Funcion	: the reply header thing :all functions need one if 
 *		  content_type is NULL then don't send content type
 * Author	:
 *
 * arguments	:
 * return	:
**/
void header(FILE *fp, char *content_type)
{
	fprintf(fp, "HTTP/1.0 200 OK\r\n");
	if (content_type)
		fprintf(fp, "Content-type:%s\r\n", content_type);
}

/*
 * Funcion	: cannot_do(fd)		unimplemented HTTP command
 *		  do_404(item, fd)	no such object
 * Author	:
 *
 * arguments	:
 * return	:
**/
void cannot_do(int fd)
{
	FILE	*fp = fdopen(fd, "w");
	fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");

	fprintf(fp, "That command is not yet implemented\r\n");
	fclose(fp);
}

void do_404(char *item, int fd)
{
	FILE	*fp = fdopen(fd, "w");
	fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");

	fprintf(fp, "The item you requested:%s\r\nis not found\r\n", item);
	fclose(fp);
}

/*
 * Funcion	: the directory listing section
 *		  isadir() uses stat, not_exist() uses stat
 *		  do_ls run ls. It should not
 * Author	:
 *
 * arguments	:
 * return	:
**/
int isadir(char *f)
{
	struct stat info;
	return (stat(f, &info) != -1 && S_ISDIR(info.st_mode));
}

int not_exist(char *f)
{
	struct stat info;
	return (stat(f, &info) == -1);
}

void do_ls(char *dir, int fd)
{
	FILE	*fp;

	fp	= fdopen(fd, "w");
	header(fp, "text/plain");
	fprintf(fp, "\r\n");
	fflush(fp);

	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);

	execlp("ls", "ls", "-1", dir, NULL);
	perror(dir);
	exit(1);
}

/*
 * Funcion	: the cgi stuff. function to check extension and one
 *		  to run the program
 * Author	:
 *
 * arguments	:
 * return	:
**/
char *file_type(char *f)
{
	char	*cp;
	if ((cp = strrchr(f, '.')) != NULL)
		return cp + 1;

	return "";
}

int ends_in_cgi(char *f)
{
	return (strcmp(file_type(f), "cgi") == 0);
}

void do_exec(char *prog, int fd)
{
	FILE *fp;

	fp	= fdopen(fd, "w");
	header(fp, NULL);
	fflush(fp);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
	execl(prog, prog, NULL);
	perror(prog);
}

/*
 * Funcion	: sends back contens after a header
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void do_cat(char *f, int fd)
{
	char	*extension	= file_type(f);
	char	*content	= "text/plain";
	FILE	*fpsock, *fpfile;
	int	c;

	if (strcmp(extension, "html") == 0)
		content	= "text/html";
	else if (strcmp(extension, "gif") == 0)
		content = "image/gif";
	else if (strcmp(extension, "jpg") == 0)
		content = "image/jpeg";
	else if (strcmp(extension, "jpeg") == 0)
		content = "image/jpeg";

	fpsock	= fdopen(fd, "w");
	fpfile	= fopen(f, "r");
	if (fpsock != NULL && fpfile != NULL) {
		header(fpsock, content);
		fprintf(fpsock, "\r\n");
		while ((c = getc(fpfile)) != EOF)
			putc(c, fpsock);
		fclose(fpfile);
		fclose(fpsock);
	}
	exit(0);
}

/*
 * Funcion	: do what the request asks for and write reply to fd
 *		  handles requsts in a new process
 *		  rq is HTTP command:GET /foo/bar.html HTTP/1.0
 * return	:
**/
void process_rq(char *rq, int fd)
{
	char	cmd[BUFSIZ], arg[BUFSIZ];

	/* create a new process and return if not the child */
	if (fork() != 0)
		return;

	strcpy(arg, "./");	/* precede args with ./ */
	if (sscanf(rq, "%s%s", cmd, arg + 2) != 2)
		return;

	if (strcmp(arg, ".//") == 0)
		strcpy(arg + strlen(arg), "index.html");

	if (strcmp(cmd, "GET") != 0)
		cannot_do(fd);
	else if (not_exist(arg))
		do_404(arg, fd);
	else if (isadir(arg))
		do_ls(arg, fd);
	else if (ends_in_cgi(arg))
		do_exec(arg, fd);
	else
		do_cat(arg, fd);
}

int main(int ac, char *av[])
{
	int	sock, fd;
	FILE	*fpin;
	char	request[BUFSIZ];

	if (ac == -1) {
		fprintf(stderr, "usage: ws portnum\n");
		exit(1);
	}

	sock	= make_server_socket(atoi(av[1]));
	if (sock == -1)
		exit(2);

	/* main loop here */
	while (1) {
		/* take a call and buffer it */
		fd	= accept(sock, NULL, NULL);
		fpin	= fdopen(fd, "r");

		/* read request */
		fgets(request, BUFSIZ, fpin);
		printf("got a call: request = %s", request);
		read_til_crnl(fpin);

		/* do what client asks */
		process_rq(request, fd);
		fclose(fpin);
	}
}
