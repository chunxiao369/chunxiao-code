LIB=-lreadline -ltermcap -ltermcap -lm -lpthread -lz

all: pxwayd pxwaycli xuexe

xuexe:test.c
	${CC} -Wall -o $@ $< ${LIB}
pxwayd:
	cc -o pxwayd pxwayd.c
pxwaycli:
	cc -o pxwaycli pxwaycli.c -lreadline -ltermcap -lcrypt

clean:
	rm -rf *.o pxwayd pxwaycli xuexe
