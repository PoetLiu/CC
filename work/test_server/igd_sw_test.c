#include "cgi.h"
#include "ParamModule2.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "wifi_cfg.h"

int write_log(const char *fmt, ...)
{
#define MAX_LOG_NUM 1000
#define LOG_FILE_NAME "/tmp/sw_test.log"
    static FILE *f = NULL;
    static int index=0;
    char cmdline[256] = {0}, index_str[256] = {0}, *p_str;
    va_list ap;
    time_t now_time;
    struct tm *p;
    int len=0, no_exist=0;

    now_time = time(NULL);
    p = localtime(&now_time);
    len = snprintf(cmdline, sizeof(cmdline), "[%d:%02d:%02d %02d:%02d:%02d]:", p->tm_year+1900, p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

     va_start(ap,fmt);
     vsprintf(cmdline+len,fmt,ap);
     va_end(ap);
     p_str = cmdline + strlen(cmdline) - 1;
     if (*p_str != '\n') {
         *(++p_str) = '\n';
         *(++p_str) = '\0';
     }

    if (access(LOG_FILE_NAME, F_OK) != 0)
        no_exist = 1;

    if (!f || no_exist) {
REOPEN:
        f = fopen(LOG_FILE_NAME , no_exist?"w":"r+");
        if(f == NULL){
            return -1;
        }
    }

    if ( ferror(f)) {
        fclose(f);
        f = NULL;
        goto REOPEN;
    }

    if (index<=0 && fgets(index_str, sizeof(index_str), f)) {
        index = atoi(index_str);
        index = (index>=MAX_LOG_NUM)||(index<0)?0:index;
    }

    if (index <= 0)
        index = 1;

    /* write log total num */
    memset(index_str, 0, sizeof(index_str));
    sprintf(index_str,"%d\n", ++index);
    fseek(f, 0, SEEK_SET);
    fwrite(index_str, sizeof(index_str), 1, f);

    /* write new num */
    fseek(f, (index-1)*sizeof(cmdline), SEEK_SET);
    fwrite(cmdline, sizeof(cmdline), 1, f);
    fflush(f);

    if (index >= MAX_LOG_NUM)
        index = 1;
    return 0;
}
#define MY_LOG(fmt, args...) do{if(1)write_log("[%s:%d] "fmt, __FUNCTION__, __LINE__, ##args);}while(0)

#define CORE_DUMP_FILE	"/tmp/core-igd_safety_wire"
#define TAR_FILE	"/tmp/bury_dead.tar.gz"
int main(void)
{

	int t, ret = 0;
	int status = 0;
	pid_t pid = 0;
	char *argv[] = {"/app/safety_wireless/bin/igd_safety_wireless_app", 0};

	nlk_start_msg(MSG_START_OK,NULL);

	// open core-dump
	system("ulimit -c unlimited");

	pid = fork();
	if( pid < 0 ){
		MY_LOG("fork failed\n");
		return -1;
	}
	else if( pid == 0 ){
		if( execvp(argv[0], argv) == -1 ){
			MY_LOG("execvp %s failed\n", argv[0]);
			exit(-1);
		}
	}

	MY_LOG("fork sucess, wait child:%d\n", pid);
	if( waitpid(pid,&status, 0) == pid ){
		MY_LOG("child process pid=%d exit, code=%x\n", pid, status);
		if(WIFEXITED(status)){
			MY_LOG("pid=%d exit nomally, code=%d\n", pid, WEXITSTATUS(status));
			ret 	= WEXITSTATUS(status);
		}
		if(WIFSIGNALED(status)){
			MY_LOG("pid=%d exit by a signal, signal=%d\n",pid,WTERMSIG(status));
			ret	= 0;
		}

		// check core-dump file
		if (access(CORE_DUMP_FILE, F_OK) != 0) {
			MY_LOG("core-dump file:%s not exist!\n", CORE_DUMP_FILE);
		} else {
			MY_LOG("core-dump file:%s exist!\n", CORE_DUMP_FILE);
		}
		system("ls /tmp/ -al >> "LOG_FILE_NAME);
		system("ps >> "LOG_FILE_NAME);
		system("free >> "LOG_FILE_NAME);
		system("tar -C /tmp/ -zcvf "TAR_FILE" "CORE_DUMP_FILE" sw_test.log sw_app.log");
	}
	else{
		MY_LOG("waitpid wrong\n");
		return -1;
	}

ret:
	return ret;
}
