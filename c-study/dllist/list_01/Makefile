#!/bin/bash
#test.sh

all:
	gcc -o list.o -c list.c
#	gcc list.c -fPIC -shared -o liblist.so
	gcc -g -I ${PWD} -o test.o -c test.c
	gcc -g -o test -L ${PWD} test.o list.o
clean:
	rm test test.o list.o liblist.so -f 
