
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
CFLAGS = -Wall -O2 -I/mnt/share_disk/program-files/CUnit-2.1-2/include/
LDFLAGS = -L/mnt/share_disk/program-files/CUnit-2.1-2/lib/ -lcunit -lcurses -static 
TARGET = xuexe
OBJS = func.o test_func.o run_test.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $^ $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
