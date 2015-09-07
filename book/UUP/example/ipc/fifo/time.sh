#!/bin/sh
# time server
while true;	do
	rm -f /tmp/time_fifo
	mkfifo /tmp/time_fifo
	date > /tmp/time_fifo
done
