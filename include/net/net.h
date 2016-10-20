#ifndef NATT_FUNC_H
#define NATT_FUNC_H
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cJSON.h"

struct address_info{
	unsigned short	port;
	unsigned char	ip[16];
};

int connect_timeout(int, const struct sockaddr *, int, struct timeval);
int get_socket(int, int);
int connect_2_address_reuse_old(const struct address_info *, int, int *, struct timeval, int);
int connect_2_address(const struct address_info *, struct timeval, int, int);
cJSON * read_json_from_fd(int, struct timeval, int);
int write_json_to_fd(int, cJSON *, struct timeval);
int write_str_2_fd(int, const char *, int, struct timeval);
int read_str_from_fd(int, char *, int, struct timeval);
unsigned char *char_bin_to_str(unsigned char );
char *bin_to_str(char *, int);
int socket_init(int, int);
int wait_client_conn(int, struct timeval, int);
int str_tail_check(const char *, int, int);

#endif
