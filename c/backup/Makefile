src=$(wildcard *.c)
obj=$(src:.c=.o)
PROG_NAME=bin2str

all: $(PROG_NAME)

$(PROG_NAME):$(obj)
	gcc $^ -o $(PROG_NAME) -g -Wall
$(obj):$(src)
	gcc -c $< -o $@
