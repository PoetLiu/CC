OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: clean all

main: ${OBJS}
	$(CC) -o $@ $^ -lpthread 

clean:
	rm -f *.o *~ main

%.o: %.c
	$(CC) -c  $<  -o $@ -lpthread  
