OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS+= -Wall -g -D__DEBUG__
CC=gcc

PROG=/home/liupeng/program/
BACKUP_DIRS=$(PROG)/backup/
INCLUDE_DIRS+= -I$(PROG)/include

.PHONY: clean all $(TARGET) tar

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIRS)
 
tar: clean
	rm -rf $(TARGET).tar.gz
	tar -zcvf $(BACKUP_DIRS)/`date +%s`.$(TARGET).tar.gz ../$(TARGET)

clean:
	rm -rf *.o $(TARGET)
		
