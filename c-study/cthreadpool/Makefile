
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
CFLAGS = -Wall -O2
LDFLAGS = 
TARGET = example
OBJS = example.o threadpool.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $^ $(LDFLAGS) -o $@ -lpthread

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(OBJS_) *~ *.d *.o $(TARGET_) $(TARGET)
