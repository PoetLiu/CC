#!/bin/bash
for ((i = 1; i < 18; i++))
do	
	if [ $i < 10 ]; then
		mkdir "ch0$i";
	else
		mkdir "ch$i";
	fi
done
