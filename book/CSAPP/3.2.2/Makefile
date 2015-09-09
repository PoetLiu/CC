prog:
	gcc -O1 -o $@ code.c main.c

code:
	gcc -O1 -S code.c

simple:
	gcc -O1 -S simple.c

clean:
	rm -rf *.o *.s prog
