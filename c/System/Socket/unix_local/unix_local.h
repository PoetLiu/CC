#ifndef	UNIX_LOCAL
#define UNIX_LOCAL

#define ERROR_NULL_PATH		-1
#define ERROR_CREAT_SOCKET	-2
#define ERROR_BIND_SOCKET	-3
#define ERROR_LISTEN_SOCKET	-4
#define ERROR_ACCEPT_SOCKET	-5
#define ERROR_CONNECT_SOCKET	-6

#define MAX_MESSAGE_LENGTH	1024
#define MAX_STR_READ		MAX_MESSAGE_LENGTH
#define MAX_STR_SEND		MAX_MESSAGE_LENGTH

#define IPC_PATH		"./.unix_local.ipc"

#endif
