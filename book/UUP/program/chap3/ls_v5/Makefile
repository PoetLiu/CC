src=$(wildcard *.c)
prog_name=myls
all: clean $(prog_name)

$(prog_name):$(src)
	gcc -o $@ $^ -g -Wall
clean:
	rm -rf $(prog_name)

