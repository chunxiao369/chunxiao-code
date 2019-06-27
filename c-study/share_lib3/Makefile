
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
CFLAGS=-Wall
LDFLAGS=-lpthread
TARGET=xuexe
OBJS=thread2.o
SO_OBJS=a.o
SO=liba.so
SOB_OBJS=b.o
SOB=libb.so
COMPILE  =  $(CC) $(CFLAGS) -MD -c -o $@ $<
COMPILE_SO  =  $(CC) $(CFLAGS) -MD -c -fPIC -o $@ $<
LINK = $(CC) -L./ $^ $(LDFLAGS) -la -lb -o $@

ALL:$(SO) $(SOB) $(TARGET)

$(SO):$(SO_OBJS)
	$(CC) $^ -shared -o $@

$(SOB):$(SOB_OBJS)
	$(CC) $^ -shared -o $@

$(TARGET):$(OBJS)
	$(LINK)

a.o:a.c
	$(COMPILE_SO)

b.o:b.c
	$(COMPILE_SO)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(SO_OBJS) $(SOB_OBJS) *~ *.d $(TARGET) $(SO) $(SOB)
