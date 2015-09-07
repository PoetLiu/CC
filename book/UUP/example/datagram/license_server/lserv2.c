/*
 *
 * Authored by Liu Peng on: Wed Nov  5 16:57:17 CST 2014
 *
 * @desc: License server server program version 2
 *	  add ticket reclaim per RECLAIM_INTERVAL
 * @func:
**/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/errno.h>

#define	MSGLEN			128	/* size of our datagrams */
#define	RECLAIM_INTERVAL	2	/* reclaim every 60 seconds */

void ticket_reclaim();

int main(int ac, char *av[])
{
	struct sockaddr_in	client_addr;
	socklen_t		addrlen = sizeof(client_addr);
	char			buf[MSGLEN];
	int			ret;
	int			sock;
	unsigned int		time_left;

	sock = setup();
	signal(SIGALRM, ticket_reclaim);	/* run ticket reclaimer */
	alarm(RECLAIM_INTERVAL);

	while (1) {
		addrlen	= sizeof(client_addr);
		ret	= recvfrom(sock, buf, MSGLEN, 0, &client_addr, \
				&addrlen);
		if (ret != -1) {
			buf[ret]	= '\0';
			narrate("GOT:", buf, &client_addr);
			time_left	= alarm(0);
			handle_request(buf, &client_addr, addrlen);
			alarm(time_left);
		} else if (errno != EINTR)
			perror("recvfrom");
	}
}
