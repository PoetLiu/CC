#!/bin/sh

if [ "$1" == "" ]; then
	echo "Nothing to be done"
else
	./$1 $2 &
	if [ "$1" == "switch" ]; then
		sleep 40
		killall app_manager market_main
	fi
	echo "---------------------DONE------------------------"
fi
