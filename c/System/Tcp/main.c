#include <stdio.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define NAS_DEBUG(fmt, args...) \
        do{if(1)printf("DEBUG %s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)
#define NAS_ERROR(fmt, args...) \
        do{if(1)printf("ERROR %s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)

static int get_socket(int reuse, int prot)
{
        int fd;

        if (prot)
                fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        else
                fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (fd < 0) {
                NAS_ERROR("socket faild\n");
                return -1;
        }
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1) {
                NAS_ERROR("set SO_REUSEADDR error\n");
                close(fd);
                return -1;
        }
        return fd;
}

static int socket_init(int port, int prot)
{
        int sk;
        struct sockaddr_in addr;
        int reuse = 1;

        if ((sk = get_socket(1, prot)) < 0)
                return -1;

	NAS_DEBUG("port:%d\n", port);
        //bind
        memset(&addr, 0, sizeof(addr));
        addr.sin_family         = AF_INET;
        addr.sin_addr.s_addr    = htonl(INADDR_ANY);
        addr.sin_port 		= htons(port);

        if (bind(sk, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                NAS_ERROR("bind failed\n");
                close(sk);
                return -1;
        }

        NAS_DEBUG("socket=%d\n", sk);
        return sk;
}

int main(int argc, char *argv[])
{
	int fd, rlen, wlen;
        struct sockaddr_in dest;
	struct timeval timeout = {10, 0};
	char buf[10];

	if (argc < 4) {
		NAS_ERROR("usage %s <local port> <dest_ip> <dest_port>\n", argv[0]);
		return -1;
	}

	fd	= socket_init(atoi(argv[1]), 1);

        dest.sin_family         = AF_INET;
        dest.sin_addr.s_addr    = inet_addr(argv[2]);
        dest.sin_port 		= htons(atoi(argv[3]));

	if (connect(fd, (struct sockaddr *)&dest, sizeof(dest)) == 0) {
		NAS_DEBUG("conneck ok\n");
		wlen = write(fd, "hello", 5);
		NAS_DEBUG("data %d write\n", wlen);
		rlen = read(fd, buf, 5);
		if (rlen < 0)
			NAS_ERROR("read error:%s\n", strerror(errno));
		buf[rlen] = '\0';
		NAS_DEBUG("data:%s, rlen:%d\n", buf, rlen);
	} else {
		NAS_ERROR("conneck faild:%s\n", strerror(errno));
	}

	return 0;
}

