TARGET=main
CC=g++
OBJS=$(patsubst %.cc,%.o,$(wildcard *.cc))
LDFLAGS+=-lmysqlclient -lpthread
DEBUG_CFLAGS=-Wall -g -DDEBUG
RELEASE_CFLAGS=-Wall -O2
PWD=`pwd`
MYLIB+=$(PWD)/mysql/libmysql.a
INCLUDE_DIR+=-I$(PWD)/mysql/

.PHONY: clean all mysqllib

ifdef DEBUG_FLAG
CFLAGS=$(DEBUG_CFLAGS)
else
CFLAGS=$(RELEASE_CFLAGS)
endif	

all:$(TARGET)

$(TARGET): mysqllib $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(MYLIB)
	@if [ -z $(DEBUG_FLAG) ];then	\
		strip $(TARGET);	\
	fi

mysqllib:
	make -C mysql

%.o:%.cc
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIR)

clean:
	make -C mysql clean
	rm -rf *.o $(TARGET)
