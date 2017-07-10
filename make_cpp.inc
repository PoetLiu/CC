OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
CFLAGS+= -Wall -g -D__DEBUG__
CC=g++

BACKUP_DIRS=$(PROG)/backup/

INC_DIRS=$(shell find $(PROG)/include -type d)
INCLUDE_DIRS=$(foreach dir,$(INC_DIRS),-I$(dir))
CFLAGS+=$(INCLUDE_DIRS)
CFLAGS+=-I$(PROG)/include/

MYLIB=$(PROG)/lib/mylib.so
LIBS+=$(MYLIB)

.PHONY: clean all tar $(DEPEND)

all: $(DEPEND) $(TARGET) $(ACT) $(LIBS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

$(MYLIB): $(PROG)/include $(PROG)/lib
	make -C $(PROG)/lib
clean:
	rm -rf *.o $(TARGET) $(CLEAN_OBJ)

tar: clean
	rm -rf $(TARGET).tar.gz
	tar -zcvf $(BACKUP_DIRS)/`date +%s`.$(TARGET).tar.gz ./
	rm -rf *.o $(TARGET)
		
