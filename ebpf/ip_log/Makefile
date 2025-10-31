
# 
# compile tool
# compile param
# link    param
# exe file
# *.o file
# compile command 
# link command
#

CC = gcc
CL = clang
CFLAGS = -Wall -O2 -g
CFLAGS_CL = -I/usr/include/x86_64-linux-gnu/
LDFLAGS = -lbpf
TARGET = ip_log
OBJS = ip_log.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
COMPILE_CL  = $(CL) $(CFLAGS) $(CFLAGS_CL) -MD -target bpf -c -o $@ $<
LINK = $(CC) $^ $(LDFLAGS) -o $@

ALL:$(TARGET) ip_log.bpf.o

$(TARGET):$(OBJS)
	$(LINK)

%.bpf.o:%.bpf.c
	$(COMPILE_CL)

%.o:%.c
	$(COMPILE)
-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
