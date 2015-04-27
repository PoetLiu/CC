PROG_NAME = test
CC = gcc

all: $(PROG_NAME)

$(PROG_NAME): test.o bin_2_str.o
	$(CC) $(CCFLAGS) $^ -o $@ -g -Wall

%.o: %.c %.h
	$(CC) -c $< -o $@

clean:
	rm -rf *.o $(PROG_NAME)
