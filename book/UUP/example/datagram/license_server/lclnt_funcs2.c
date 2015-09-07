/*
 *
 * Authored by Liu Peng on: Wed Nov  5 16:25:26 CST 2014
 * lcnl_funcs2.c
 * @desc: for the client of the license server
 *	  add do_validate for server crash, recheck client ticket
 * @func:
**/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 * Important variables used throughout
 */
static int	pid	= -1;		/* our PID */
static int	sd	= -1;		/* our communications socket */
static struct sockaddr	serv_addr;	/* Server address */
static socklen_t	serv_alen;	/* length of address */
static char		ticket_buf[128]; /* Buffer to hold our ticket */
static int		have_ticket = 0; /* Set when we have a ticket */

#define MSGLEN		128		/* Size of our datagrams */
#define SERVER_PORTNUM	2020		/* Our server's port number */
#define HOSTLEN		512
#define oops(s)	{perror(s);exit(1);}

char *do_transaction();
void narrate(char * msg1, char *msg2);
void syserr(char *msg);

/*
 * Funcion	: get pid, socket, and address of license server
 * IN no args
 * return	: nothing, dies on error
 * notes	: assumes server is on same host as client
**/
void setup()
{
	char	hostname[BUFSIZ];

	pid	= getpid();			/* for ticks and msgs */
	sd	= make_dgram_client_socket();	/* to talk to server */

	if (sd == -1)
		oops("cannot create socket");
	gethostname(hostname, HOSTLEN);		/* server on same host */
	make_internet_address(hostname, SERVER_PORTNUM, &serv_addr);
	serv_alen	= sizeof(serv_addr);
}

void shut_down()
{
	close(sd);
}

/*
 * Funcion	: get a ticket from the license server
 *
 * Author	:
 *
 * arguments	:
 * return	: 0 for success, -1 for failure
**/
int get_ticket()
{
	char	*response;
	char	buf[MSGLEN];

	if (have_ticket)		/* don't be greedy */
		return (0);

	sprintf(buf, "HELO %d", pid);	/* compose request */

	if ((response = do_transaction(buf)) == NULL)
		return -1;

	/* parse the response and see if we got a ticket 
	 * on success, the message is :TICK ticket-string
	 * on failure, the message is :FAIL failure-msg
	 */
	if (strncmp(response, "TICK", 4) == 0) {
		strcpy(ticket_buf, response + 5);	/* grab ticket-id */
		have_ticket	= 1;
		narrate("got ticket", ticket_buf);
		return 0;
	}

	if (strncmp(response, "FAIL", 4) == 0)
		narrate("Could not get ticket", response);
	else
		narrate("Unknown message:", response);
	return -1;
}

/*
 * Funcion	: release ticket
 *		  Give a ticket back to the server
 * Author	:
 *
 * arguments	:
 * return	: 0 for success, -1 for failure
**/
int release_ticket()
{
	char	buf[MSGLEN];
	char	*response;

	if (!have_ticket)			/* don't have a ticket */
		return 0;

	sprintf(buf, "GBYE %s", ticket_buf);	/* compose message */
	if ((response = do_transaction(buf)) == NULL)
		return -1;

	/* examine response 
	 * success: THNX info-string
	 * failure: FAIL error-string
	 */
	if (strncmp(response, "THNX", 4) == 0) {
		narrate("released ticket OK", "");
		return 0;
	}

	if (strncmp(response, "FAIL", 4) == 0)
		narrate("release failed", response + 5);
	else
		narrate("Unknown message:", response);
	return -1;
}

/*
 * Funcion	: send a request to check client's ticket
 *		  avoid mess while server crash and restart
 * Author	:
 *
 * arguments	:
 * return	:
**/
void do_validate()
{
	char	buf[MSGLEN];
	char	*response;

	if (!have_ticket)
		return 0;

	sprintf(buf, "VALD %s", ticket_buf);
	if ((response = do_transaction(buf)) == NULL)
		return -1;

	if (strncmp(response, "GOOD", 4) == 0) {
		narrate("good valid ticket", "");
		return 0;
	}

	if (strncmp(response, "FAIL", 4) == 0) {
		narrate("fail invalid ticket", response);
	} else
		narrate("Unknown message:", response);

	return -1;
		
}

/*
 * Funcion	: Send a request to the server and get a response back
 * IN msg_p	message to send
 *
 * return	: pointer fo message string, or NULL for error
 * note		: pointer returned is to static storage
 *		  overwritten by each successive call
 * caution	: for extra security, compare retaddr to serv_addr(why?)
**/
char *do_transaction(char *msg)
{
	static char buf[MSGLEN];
	struct sockaddr retaddr;
	socklen_t	addrlen = sizeof(retaddr);
	int	ret;

	ret	= sendto(sd, msg, strlen(msg), 0, &serv_addr, serv_alen);
	if (ret == -1) {
		syserr("sendto");
		return NULL;
	}

	/* Get the response back */
	ret	= recvfrom(sd, buf, MSGLEN, 0, &retaddr, &addrlen);
	if (ret == -1) {
		syserr("recvfrom");
		return NULL;
	}

	/* Now return the message itself */
	return buf;
}

/*
 * Funcion	: print messages to stderr for debugging and demo purposes
 * IN msg1, msg2: strings to print along with pid and title
 * return	: nothing , dies on error
**/
void narrate(char * msg1, char *msg2)
{
	fprintf(stderr, "CLIENT[%d]:%s%s\n", pid, msg1, msg2);
}

void syserr(char *msg)
{

	char buf[MSGLEN];
	sprintf(buf, "CLIENT[%d]:%s ", pid, msg);
	perror(buf);
}
