
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
CFLAGS=-Wall -O2
LDFLAGS=-lpthread
TARGET=xuexe
OBJS=thread.o
SO_OBJS=a.o
SO=liba.so
COMPILE  =  $(CC) $(CFLAGS) -MD -c -o $@ $<
COMPILE_SO  =  $(CC) $(CFLAGS) -MD -c -fPIC -o $@ $<
LINK = $(CC) -L./ $^ $(LDFLAGS) -la -o $@

ALL:$(SO) $(TARGET)

$(SO):$(SO_OBJS)
	$(CC) $^ -shared -o $@

$(TARGET):$(OBJS)
	$(LINK)

a.o:a.c
	$(COMPILE_SO)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(SO_OBJS) *~ *.d $(TARGET) $(SO)
