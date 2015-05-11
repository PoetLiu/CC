TARGET=a.out
MYLIB=libmysql.a
CC=g++
AR=ar
OBJS=$(patsubst %.cc,%.o,$(wildcard *.cc))
LDFLAGS+=-lmysqlclient

ifdef DEBUG_FLAG
	CFLAGS+=-Wall -g -DDEBUG
else
	CFLAGS+=-Wall -O2
endif

.PHONY: clean all 

all:$(TARGET) $(MYLIB)

$(MYLIB):$(OBJS)
	$(AR) cr $@ $^

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o:%.cc
	$(CC) -c $< -o $@ $(CFLAGS)

mysql.o:mysql.h

clean:
	rm -rf *.o $(TARGET) $(MYLIB)
