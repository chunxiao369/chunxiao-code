
# 
# compile tool
# compile param
# link    param
# exe file
# *.o file
# compile command 
# link command
#

#CC = mips64-octeon-linux-gnu-gcc
CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = 
TARGET = xuexe
OBJS = a.o b.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

aaaa:=first

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)
bbb:=$(aaaa)

a.o:a.c
	$(COMPILE)
	echo $(aaaa)
	echo bbb=$(bbb)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
aaaa:=second


bbb:=$(aaaa)

b.o:b.c
	echo bb1=$(bbb)
