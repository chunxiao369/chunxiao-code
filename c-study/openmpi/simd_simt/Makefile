EXECS=simd simt
CC=gcc

all: ${EXECS}

simd: simd.c
	${CC} -c -o simd.o simd.c
	${CC} -o simd simd.o

simt: simt.c
	${CC} -c -o simt.o simt.c
	${CC} simt.o -lpthread -o simt

clean:
	rm -f ${EXECS} *.o

