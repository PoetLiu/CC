INCLUDE_DIRS+= -I$(PROG)/include/list 
INCLUDE_DIRS+= -I$(PROG)/include/queue
TARGET=mysniffer.out
LIBQUEUE=$(PROG)/lib/queue/queue.o
DEPEND=$(LIBQUEUE)
LIBS+=$(LIBQUEUE)
#CFLAGS+=-D__DISP_ALL
LDFLAGS+=-lpthread

include $(PROG)/make.inc 

$(LIBQUEUE):
	make -C $(PROG)/lib/queue

