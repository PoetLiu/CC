#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define ARGC_MAX 10
#define CMDLINE_SIZE 256

#define PS_DEBUG(fmt, args...) do{if(1)console_printf("DEBUG:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)
#define PS_ERROR(fmt, args...) do{if(1)console_printf("ERROR:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)

static void _close_parent_fds()
{
	int i;

	for (i = 3; i < 1024; i++)
		close(i);
}

pid_t ps_start_argv(char * argv[])
{
	pid_t pid;

	pid = fork();

	if (pid < 0) {
		PS_ERROR("fork error\n");
		return 0;
	}

	if (pid == 0) {
		_close_parent_fds();

		if (execvp(argv[0], argv) == -1) {
			PS_ERROR("execvp error") ;
			exit(-1);
		}

		return 0;
	}

	return pid;
}

pid_t ps_start(char * cmdline)
{
	pid_t pid;

	pid = fork();

	if (pid < 0) {
		PS_ERROR("fork error\n");
		return 0;
	}

	if (pid == 0) {
		int i = 0 ;
		char *tmp=NULL;
		char *argv[1024] = {0};
		char buff[2048] = {0};
		char *param = NULL;

		PS_DEBUG("cmdline='%s'\n",cmdline);

		strncpy(buff,cmdline,sizeof(buff));
		param = &buff[0];
		while((tmp = strchr(param,' '))!=NULL){
				*tmp='\0';
				if(*(tmp-1)!='\0')
						argv[i++] = param;
				param=tmp;
				param++;
		}
		argv[i++] = param;
		argv[i] = NULL;

		_close_parent_fds();

		if (execvp(argv[0], argv) == -1) {
			PS_ERROR("execvp error") ;
			exit(-1);
		}

		return 0;
	}

	return pid;
}

int ps_wait(pid_t pid, int * status)
{
	int s;

	if(waitpid(pid,&s,0) == pid){
		if(WIFEXITED(s)){
			PS_DEBUG("pid=%d exit nomally, code=%d\n",pid,WEXITSTATUS(status));
		}
		if(WIFSIGNALED(s)){
			PS_DEBUG("pid=%d exit by a signal, signal=%d\n",pid,WTERMSIG(status));
		}

		if (status)
			*status = s;
		return pid;
	}

	if (status)
		*status = 0;
	return 0;
}

int ps_wait_nohang(pid_t pid, int * status)
{
	int s;

	if(waitpid(pid,&s,WNOHANG) == pid){
		if(WIFEXITED(s)){
			PS_DEBUG("pid=%d exit nomally, code=%d\n",pid,WEXITSTATUS(status));
		}
		if(WIFSIGNALED(s)){
			PS_DEBUG("pid=%d exit by a signal, signal=%d\n",pid,WTERMSIG(status));
		}

		if (status)
			*status = s;
		return pid;
	}

	if (status)
		*status = 0;
	return 0;
}

void cmdline_to_argv(char * cmdline, char * argv[])
{
	int i                   = 0;
	char *tmp               = NULL;
	static char buff[CMDLINE_SIZE] = {0};
	char *param             = NULL;

	assert(cmdline);
	assert(argv);

	//NAS_DEBUG("cmdline='%s'\n",cmdline);

	memset(buff, 0, sizeof(buff));
	strncpy(buff,cmdline,CMDLINE_SIZE);
	param = &buff[0];
	while( (tmp = strchr(param,' ')) != NULL ){
            *tmp='\0';
            if( *(tmp-1) != '\0' /*FIXME: ?*/){
				if( i >= (ARGC_MAX - 1) ){
					PS_ERROR("too many args\n");
					return;
				}
				argv[i++] = param;
			}
            param = tmp;
            param++;
    }
    argv[i++] = param;
    argv[i] = NULL;
}

int eval_timeout(int timeout, char *fmt, ...)
{
	int t;
	int status = 0;
	pid_t pid = 0;
	char cmdline[CMDLINE_SIZE] = {0};
	char * argv[ARGC_MAX] = {NULL};
	va_list ap;

	va_start(ap,fmt);
	vsprintf(cmdline,fmt,ap);
	va_end(ap);

	//NAS_DEBUG("cmdline='%s'\n", cmdline);

	cmdline_to_argv(cmdline, argv);

	pid = fork();
	if( pid < 0 ){
		PS_ERROR("fork failed\n");
		return -1;
	}
	else if( pid == 0 ){
		if( execvp(argv[0], argv) == -1 ){
			PS_ERROR("execvp %s failed\n", cmdline);
			exit(-1);
		}
	}

	t = 0;
	while( t < timeout ){
		if( waitpid(pid, &status, WNOHANG) == pid  ){
			if(WIFEXITED(status)){
				//NAS_DEBUG("pid=%d exit nomally, code=%d\n",pid,WEXITSTATUS(status));
				return WEXITSTATUS(status);
			}
			if(WIFSIGNALED(status)){
				PS_DEBUG("pid=%d exit by a signal, signal=%d\n",pid,WTERMSIG(status));
			}

			return 0;
		}
		sleep(1);
		t++;
	}

	kill(pid,SIGTERM);

	if( waitpid(pid,&status, 0) == pid ){
		if(WIFEXITED(status)){
			//NAS_DEBUG("pid=%d exit nomally, code=%d\n",pid,WEXITSTATUS(status));
			return WEXITSTATUS(status);
		}
		if(WIFSIGNALED(status)){
			PS_DEBUG("pid=%d exit by a signal, signal=%d\n",pid,WTERMSIG(status));
		}

		return 0;
	}
	else{
		PS_ERROR("something wrong\n");

		return -1;
	}
}

int read_file_to_buffer(char * file/*file name*/, char * buf, int len/*buffer len*/)
{
	int fd;
	off_t size;
	int ret = 0;

	memset(buf, 0, len);

	fd = open(file, O_RDWR, 0666);
	if( fd < 0 ){
		PS_ERROR("open '%s' failed\n", file);
		return -1;
	}

	size = lseek(fd, 0, SEEK_END);	//to end
	if( size < 0 ){
		PS_ERROR("lseek failed\n");
		ret = -1;
	}
	else{
		if( size > len ){
			PS_ERROR("buffer size is too small, %d < %ld\n", len, size);
			ret = -1;
		}
		else{
			if( lseek(fd, 0, SEEK_SET) < 0 ){	//to start
				PS_ERROR("lseek failed\n");
				close(fd);
				return -1;
			}
			size_t _size = read(fd,buf,size);
			if( _size != size ){
				PS_ERROR("read error\n");
				ret = -1;
			}
		}
	}
	close(fd);

	return ret;
}

int eval_capture_output(char * buf, int len, char * fmt, ...)
{
	int status = 0;
	pid_t pid = 0;
	char cmdline[CMDLINE_SIZE] = {0};
	char * argv[ARGC_MAX] = {NULL};
	char * file_out;
	va_list ap;

	va_start(ap,fmt);
	vsprintf(cmdline,fmt,ap);
	va_end(ap);

	cmdline_to_argv(cmdline, argv);

	file_out = tempnam("/tmp", "owtmp");
	if( !file_out ){
		PS_ERROR("generate tmp file name failed\n");
		return -1;
	}

	pid = fork();
	if( pid < 0 ){
		PS_ERROR("fork failed\n");
		return -1;
	}
	else if( pid == 0 ){
		int _std_out;
		
		_std_out = open(file_out,O_RDWR | O_CREAT, 0666);
		if( _std_out < 0 ){
			PS_ERROR("open '%s' failed\n", file_out);
			exit(-1);
		}

		if( dup2(_std_out, 1) == -1 ){
			PS_ERROR("dup2 failed\n");
			close(_std_out);
			exit(-1);
		}

		if( execvp(argv[0], argv) == -1 ){
			PS_ERROR("execvp %s failed\n", cmdline);
			exit(-1);
		}
		close(_std_out);
	}

	if( waitpid(pid, &status, 0) == pid ) {

		read_file_to_buffer(file_out, buf, len);
		unlink(file_out);

		if(WIFEXITED(status)){
			PS_DEBUG("pid=%d exit nomally, code=%d\n",pid,WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status)){
			PS_ERROR("pid=%d exit by a signal, signal=%d\n",pid,WTERMSIG(status));
		}

		return 0;
	}
	else {
		PS_ERROR("something wrong\n");
		return -1;
	}
}
