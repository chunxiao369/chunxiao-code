all:
#mips64-octeon-linux-gnu-gcc -Wall -o ethernet-test com_octpow_sendpacket.c
	flex the.l
	bison -d -v the.y
#lex the.l
#yacc -d the.y
	gcc -c lex.yy.c
	gcc -c the.tab.c
	cd ./octpow;gcc -c com_octpow_sendpacket.c;cd ..
	ar -rc libyacclex.a lex.yy.o the.tab.o ./octpow/com_octpow_sendpacket.o
	gcc -o ac_cli ac_cli.c -L . -lyacclex
clean:
	rm -f lex.yy.c y.tab.c y.tab.h ac_cli
	rm -f *.o *.d *.a ./octpow/*.o
	rm -f *.tab.* lex.yy.c the.output
