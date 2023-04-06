
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
CFLAGS = -Wall -O2 -I/home/lance/install/include/hs/
LDFLAGS = -L/home/lance/install/lib/
#TARGET = simple_grep
TARGET = capture
OBJS = capture.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@ -lhs -lpcre

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
