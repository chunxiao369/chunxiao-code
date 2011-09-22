all:
	#mips64-octeon-linux-gnu-gcc -Wall -o ethernet-test com_octpow_sendpacket.c
	lex the.l
	yacc -d the.y
	gcc lex.yy.c y.tab.c y.tab.h -o xucli -ll
clean:
	rm -f ethernet-test
	rm -f lex.yy.c y.tab.c y.tab.h xucli
