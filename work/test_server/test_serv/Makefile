# $Id: Makefile,v 1.1.1.1 2006/03/27 07:45:07 taof Exp $
include $(MAKEFILE_TOP_INCLUDE)

INCLUDE_DIRS = -I $(USERDIR)/switch/include/ -I $(USERDIR)/switch/libzebos/include/ -I $(USERDIR)/projects/5600/include/
INCLUDE_DIRS += -I $(SWITCH)/igd/base64
INCLUDE_DIRS += -I $(SWITCH)/igd/cjson

LIBIPC = $(USERDIR)/switch/libzebos/objs/nt_ipc_client.o
IGD_LIB = $(USERDIR)/switch/igd/libigd.a
LDFLAGS = $(SWITCH)/share/libnorouter.so -lm

CFLAGS = -g  -pipe -Wall -Os

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

PROG=test_serv

MAKE=make

.PHONY: clean all

all: $(PROG)
	rm -rf /tmp/$(PROG)
	cp $(PROG) /tmp
	(echo "get "$(PROG) | telnet 192.168.0.1 8888) || echo ""

$(PROG): ${OBJS}
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) -L${SWITCH} ${TOP_PROJECT_PATH}/user/switch/libzebos.a -lpthread $(IGD_LIB)
	$(STRIP) $(PROG) 

clean:
	rm -f *.o *~ $(PROG)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_DIRS) -lpthread $<  -o $@ 
