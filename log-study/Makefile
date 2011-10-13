
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
#CC = ppc_82xx-gcc
CC = gcc
CFLAGS = -Wall -O2 -I /mnt/share_disk/program-files/log4c/include/
LDFLAGS = -L /mnt/share_disk/program-files/log4c/lib/
TARGET = xuexe
OBJS = a.o 
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $^ $(LDFLAGS) -o $@ -llog4c

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
