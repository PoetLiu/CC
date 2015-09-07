src=$(wildcard *.c)
obj=$(src:.c=.o)
prog_name=tw2
LDFLAGS=-lpthread
libs+=./socklib.o

.HONY: $(prog_name)
$(prog_name): $(obj)
#echo "src:" $(src)
#echo "obj:" $(obj)
	$(CC) -o $@ twebserv.o $(libs) $(LDFLAGS)
