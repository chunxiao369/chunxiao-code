CC=gcc
CFLAGS=-Wall -O2
LDFLAGS=
TARGET=array_list
OBJS=$(TARGET).o
COMPILE  =  $(CC) $(CFLAGS) -g -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d $(TARGET)
