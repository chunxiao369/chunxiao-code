
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
CFLAGS = -Wall -O2 -I/mnt/share_disk/program-files/libpcap/include/
LDFLAGS =  -L/mnt/share_disk/program-files/libpcap/lib/ -lpcap
TARGET = xuexe
OBJS = packet.o dns.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $(OBJS) $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
