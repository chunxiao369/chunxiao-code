
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

#CC = mipsisa64-octeon-elf-gcc
CC = gcc
CFLAGS = -Wall -O2 -I/home/chunxiao/proto-buffc/include/
#LDFLAGS = libprotobuf-c.a 
#LDFLAGS = /mnt/share_disk/program-files/protobuf-c-0.14/src/google/protobuf-c/libprotoc.a
LDFLAGS = /home/chunxiao/proto-buffc/lib-i386/libprotobuf-c.a
TARGET = xuexe
OBJS = amessage.pb-c.o test2.o 
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $(OBJS) $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)
%.pb-c.c:%.proto
	/home/chunxiao/proto-buffc/bin/protoc-c amessage.proto --c_out=./

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET) *pb-c.[ch]
