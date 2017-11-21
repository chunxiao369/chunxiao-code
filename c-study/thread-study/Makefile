
# 
# compile tool
# compile param
# link    param
# exe file
# *.o file
# compile command 
# link command
#

CC=gcc
#CC=ppc_82xx-gcc
CFLAGS=-Wall -O2 -g
LDFLAGS=-lpthread
TARGET=xuexe
OBJS=thread08.o
COMPILE  =  $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d $(TARGET)
