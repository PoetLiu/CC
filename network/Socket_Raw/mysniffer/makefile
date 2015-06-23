INCLUDE_DIRS+= -I$(PROG)/include/list 
INCLUDE_DIRS+= -I$(PROG)/include/queue
TARGET=mysniffer.out
LIBQUEUE=$(PROG)/lib/queue/queue.o
DEPEND=libqueue
LIBS+=$(LIBQUEUE)
#CFLAGS+=-D__DISP_ALL
LDFLAGS+=-lpthread

include $(PROG)/make.inc 

libqueue:
	make -C $(PROG)/lib/queue

