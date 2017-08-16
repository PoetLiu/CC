#!/bin/sh
debug_flag=0

while getopts ":d" opt
do 
	case $opt in
	d) debug_flag=1 ;;
	esac
done

echo "----------Begin compiling-------"
echo "cleaning old objs..."
make clean

if [ $debug_flag -eq 1 ]
then
	echo "making debug version..."
	export DEBUG_FLAG=1
	make
	unset DEBUG_FLAG
else
	echo "making release version..."
	make
fi

echo "------------Done-----------------"
