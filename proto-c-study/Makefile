
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
PROTOC_DIR=/mnt/share_disk/program-files/proto-buffc/
CC = gcc
CFLAGS = -Wall -O2 -I${PROTOC_DIR}include/
LDFLAGS = ${PROTOC_DIR}lib-x86_64/libprotobuf-c.a
TARGET = xuexe
OBJS = amessage.pb-c.o test3.o 
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $(OBJS) $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)
%.pb-c.c:%.proto
	${PROTOC_DIR}bin/protoc-c amessage.proto --c_out=./

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET) *pb-c.[ch]
