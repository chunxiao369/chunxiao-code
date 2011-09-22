#CROSS_COMPILE=mips64-octeon-linux-gnu-
CROSS_COMPILE=
CC=${CROSS_COMPILE}gcc
STRIP=${CROSS_COMPILE}strip

LIB=-lreadline -ltermcap

TARGET=pxwayd pxwaycli xuexe

all:${TARGET}
	${STRIP} ${TARGET}

pxwaycli:pxwaycli.c
	${CC} -Wall -o pxwaycli pxwaycli.c ${LIB}

xuexe:test.c
	${CC} -Wall -o $@ $< ${LIB}

strip:${TARGET}
	${STRIP} ${TARGET}

touch:
	touch *.c

clean:
	rm -rf ${TARGET} ${JUNK}

