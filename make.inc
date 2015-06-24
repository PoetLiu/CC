OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS+= -Wall -g -D__DEBUG__
CC=gcc

PROG=/home/liupeng/program/
BACKUP_DIRS=$(PROG)/backup/

.PHONY: clean all tar $(DEPEND)

all: $(DEPEND) $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIRS)
 
clean:
	rm -rf *.o $(TARGET)

tar: clean
	rm -rf $(TARGET).tar.gz
	tar -zcvf $(BACKUP_DIRS)/`date +%s`.$(TARGET).tar.gz ./
	rm -rf *.o $(TARGET)
		
	
