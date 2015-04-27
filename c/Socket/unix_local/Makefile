CFLAGS=-I./
CC=gcc

all: client server

client: client.c unix_local.h
	$(CC) client.c -o client $(CFLAGS)

server: server.c unix_local.h
	$(CC) server.c -o server $(CFLAGS)

.PHONY: clean
clean:
	rm ./client
	rm ./server
