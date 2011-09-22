
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
TARGET = write_exe
OBJS = write.o
TARGET_ = read_exe
OBJS_ = read.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

ALL:$(TARGET) $(TARGET_)

$(TARGET):$(OBJS)
	$(LINK)

$(TARGET_):$(OBJS_)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(OBJS_) *~ *.d *.o $(TARGET_) $(TARGET)
