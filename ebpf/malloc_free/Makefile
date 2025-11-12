
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
CFLAGS_CL = -I/usr/include/x86_64-linux-gnu/  -fno-merge-all-constants
LDFLAGS = -lbpf -lelf
TARGET = malloc_free 
OBJS = malloc_free.o
BPFTOOL = /home/lance/Project/open-source/bpftime/example/malloc/.output/bpftool/bootstrap/bpftool
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
COMPILE_CL  = $(CL) -target bpf $(CFLAGS) $(CFLAGS_CL) -c -o $@ $<
LINK = $(CC) $^ $(LDFLAGS) -o $@

ALL:malloc_free.bpf.o $(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.bpf.o:%.bpf.c
	$(COMPILE_CL)
	$(BPFTOOL) gen skeleton malloc_free.bpf.o > malloc_free.skel.h


%.o:%.c
	$(COMPILE)
-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET) malloc_free.skel.h
