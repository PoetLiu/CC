/*
 *
 * Authored by Liu Peng on: Fri Nov  7 13:33:14 CST 2014
 *
 * @desc	: a threaded minimal wed server (version 0.2)
 * @usage	: tws portnumber
 * @features	: support the GET command only
 *		  runs in the current directory
 *		  creates a thread to handle each request
 * @building	: cc twebserv.c socklib.c -lpthread -o twebserv
**/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>

void	*handle_call(void *);
void	setup(pthread_attr_t *attrp);
void	*handle_call(void *fdptr);
void	skip_rest_of_header(FILE *fp);
void	process_rq(char *rq, int fd);
void	sanitize(char *str);
int 	built_in(char *arg, int fd);
int	http_reply(int, FILE **, int, char *, char *, char *);
void	not_implemented(int fd);
void	do_404(char *item, int fd);
int	isadir(char *f);
int	not_exist(char *f);
void	do_ls(char *dir, int fd);
char	*file_type(char *f);
void	do_cat(char *f, int fd);

/* server facts here */
time_t	server_started;
int	server_bytes_sent;
int	server_requests;

int main(int ac, char *av[])
{
	int		sock, fd;
	int		*fdptr;
	int		len;
	struct sockaddr_in cli_addr;
	pthread_t	worker;
	pthread_attr_t	attr;

	if (ac == 1) {
		fprintf(stderr, "usage:tws portnum\n");
		exit(1);
	}

	sock	= make_server_socket(atoi(av[1]));
	if (sock == -1) {
		perror("making socket");
		exit(2);
	}

	setup(&attr);

	len	= sizeof(struct sockaddr);
	/* main loop here: take call, handle call int new thread */
	while (1) {
		fd		= accept(sock, (struct sockaddr *)&cli_addr, &len);
		//fd		= accept(sock, NULL, NULL);
		printf("get a call from (%s:%d)\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		server_requests++;
		fdptr		= malloc(sizeof(int));
		*fdptr		= fd;
		pthread_create(&worker, &attr, handle_call, fdptr);
	}

}	

/*
 * initialize the status variable and 
 * set the thread attribute to detached
 */
void setup(pthread_attr_t *attrp)
{
	pthread_attr_init(attrp);
	pthread_attr_setdetachstate(attrp, PTHREAD_CREATE_DETACHED);

	time(&server_started);
	server_requests		= 0;
	server_bytes_sent	= 0;

}

void *handle_call(void *fdptr)
{
	FILE	*fpin;
	char	request[BUFSIZ];
	int	fd, len, len_all;
	char 	*msg	= "\{\"scmd\":\"challenge\",\"natt_challenge\":\"sdfdfsdfsfdfsf\"}";

	fd	= *(int *)fdptr;
	free(fdptr);			/* get fd from arg */
#if 1
	len	= strlen(msg);
	len	= htonl(len);
	write(fd, &len, sizeof(int));
	write(fd, msg, strlen(msg)); 

	read(fd, request, BUFSIZ);
	printf("got a call on %d head:%d msg:%s\n", fd, ntohl(*(int*)request), request + sizeof(int));
#else	
	fpin	= fdopen(fd, "r");	/* buffer input */
	fgets(request, BUFSIZ, fpin);	/* read client request */

	printf("got a call on %d: request = %s", fd, request);
	skip_rest_of_header(fpin);

	process_rq(request, fd);	/* process client rq */
	fclose(fpin);
#endif
	close(fd);
}


/*
 * Funcion	: skip over all request info until a CRNL is seen
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void skip_rest_of_header(FILE *fp)
{
	char	buf[BUFSIZ];

	while (fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0);

}

/*
 * Funcion	: do what the request asks for and write reply to fd
 *		  handles request in a new process
 * arguments	: rq is HTTP command:GET /foo/bar.html HTTP/1.0
 * return	:
**/
void process_rq(char *rq, int fd)
{
	char	cmd[BUFSIZ], arg[BUFSIZ];

	if (sscanf(rq, "%s%s", cmd, arg) != 2)
		return;
	sanitize(arg);
	printf("sanitized version is %s\n", arg);

	if (strcmp(cmd, "GET") != 0)
		not_implemented(fd);
	else if (built_in(arg, fd))
		;
	else if (not_exist(arg))
		do_404(arg, fd);
	else if (isadir(arg))
		do_ls(arg, fd);
	else
		do_cat(arg, fd);
}

/*
 * Funcion	: make sure all paths below the current directory
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void sanitize(char *str)
{
	char	*src, *dest;

	src	= str;
	dest	= str;

	while (*src) {
		if (strncmp(src, "/../", 4) == 0)
			src	+= 3;
		else if (strncmp(src, "//", 2) == 0)
			src++;
		else
			*dest++	= *src++;
	}

	*dest	= '\0';
	if (*str == '/')
		strcpy(str, str + 1);

	if (str[0] == '\0' || strcmp(str, "./") == 0 || 	\
		strcmp(str, "./..") == 0)
		strcpy(str, ".");
}

/*
 * Funcion	: handle built-in URLs here. Only one so far is "status"
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
int  built_in(char *arg, int fd)
{
	FILE	*fp;

	if (strcmp(arg, "status") != 0)
		return 0;

	http_reply(fd, &fp, 200, "OK", "text/plain", NULL);

	fprintf(fp, "Server started:%s", ctime(&server_started));
	fprintf(fp, "Total requests:%d\n", server_requests);
	fprintf(fp, "Bytes sent out:%d\n", server_bytes_sent);
	fclose(fp);
	return 1;
}

int http_reply(int fd, FILE **fpp, int code, char *msg, char *type,	\
	char *content)
{
	FILE	*fp	= fdopen(fd, "w");
	int	bytes	= 0;

	if (fp	!= NULL) {
		bytes	= fprintf(fp, "HTTP/1.0 %d %s\r\n", code, msg);
		bytes	+= fprintf(fp, "Content-type:%s\r\n\r\n", type);
		if (content)
			bytes	+= fprintf(fp, "%s\r\n", content);

	}
	fflush(fp);

	if (fpp)
		*fpp	= fp;
	else
		fclose(fp);
	return bytes;
}

void not_implemented(int fd)
{
	http_reply(fd, NULL, 501, "Not Implementd", "text/plain",\
		"That command is not implemented");
}

void do_404(char *item, int fd) 
{
	http_reply(fd, NULL, 404, "Not Found", "text/plain", 	\
		"The item you seek is not here");
}

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
	DIR	*dirptr;
	struct dirent *direntp;
	FILE	*fp;
	int	bytes	= 0;

	bytes	= http_reply(fd, &fp, 200, "OK", "text/plain", NULL);
	bytes	+= fprintf(fp, "Listing of Directory %s\n", dir);

	if ((dirptr = opendir(dir)) != NULL) {
		while (direntp = readdir(dirptr)) {
			bytes	+= fprintf(fp, "%s\n", direntp->d_name);
		}
		closedir(dirptr);
	}
	fclose(fp);
	server_bytes_sent	+= bytes;
}

/*
 * Funcion	: functions to cat files here
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
char *file_type(char *f)
{
	char *cp;
	if ((cp = strrchr(f, '.')) != NULL)
		return cp + 1;
	return "   -";
}

/*
 * Funcion	: sends header then the contents
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void do_cat(char *f, int fd)
{
	char 	*extension	= file_type(f);
	char	*type		= "text/plain ";
	FILE	*fpsock, *fpfile;
	int	c;
	int	bytes	= 0;

	if (strcmp(extension, "html") == 0)
		type	= "text/html";
	else if (strcmp(extension, "gif") == 0)
		type	= "image/gif";
	else if (strcmp(extension, "jpg") == 0)
		type	= "image/jpeg";
	else if (strcmp(extension, "jped") == 0)
		type	= "image/jpeg";

	fpsock	= fdopen(fd, "w");
	fpfile	= fopen(f, "r");
	if (fpsock != NULL && fpfile != NULL) {
		bytes	= http_reply(fd, &fpsock, 200, "OK", type, NULL);
		while ((c = getc(fpfile)) != EOF) {
			putc(c, fpsock);
			bytes++;
		}
		fclose(fpfile);
		fclose(fpsock);
	}
	server_bytes_sent	+= bytes;
}
