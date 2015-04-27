OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS+= -Wall -g -D__DEBUG__
CC=gcc

.PHONY: clean all $(TARGET) tar

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIRS)
 
tar: clean
	rm -rf $(TARGET).tar.gz
	cd ../backup && tar -zcvf `date +%s`.$(TARGET).tar.gz ../$(TARGET)

clean:
	rm -rf *.o $(TARGET)
		
