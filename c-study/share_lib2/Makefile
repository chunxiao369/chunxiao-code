
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
CFLAGS=-Wall -O0
LDFLAGS=-lpthread
TARGET=xuexe
OBJS=thread.o
SO_OBJS=a.o
LIB_OBJS=b.o
SO=liba.so
LIBS=libb.a
COMPILE     = $(CC) $(CFLAGS) -MD -c -o $@ $<
COMPILE_SO  = $(CC) $(CFLAGS) -MD -c -fPIC -o $@ $<
COMPILE_LIB = $(AR) -cr $@ $^
LINK = $(CC) -L./ $^ $(LDFLAGS) -la $(LIBS) -ldl -o $@

ALL:$(LIBS) $(SO) $(TARGET)

$(LIBS):$(LIB_OBJS)
	$(COMPILE_LIB)

$(SO):$(SO_OBJS)
	$(CC) -Wl,-Bsymbolic $^ $(LIBS) -shared -o $@
#	$(CC) $^ $(LIBS) -shared -o $@

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
	rm -f $(OBJS) $(SO_OBJS) *~ *.d $(TARGET) $(SO) $(LIBS) $(LIB_OBJS)
