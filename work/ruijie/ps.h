#ifndef _IGD_PS__H_
#define _IGD_PS__H_

pid_t ps_start_argv(char * argv[]);
pid_t ps_start(char *cmdline);
int ps_wait(pid_t pid,int * status);
int ps_wait_nohang(pid_t pid, int * status);

int eval_timeout(int timeout, char *fmt, ...);
int eval_capture_output(char * buf, int len, char * fmt, ...);

#endif /*_IGD_PS__H_*/
