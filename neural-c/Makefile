CC=gcc
CFLAGS=-Wall -O2
LDFLAGS= -lm
TARGET=neural_test
OBJS=data_load.o  main.o  nn.o
COMPILE=$(CC) $(CFLAGS) -MD -c -o $@ $<
LINK=$(CC) $^ $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d $(TARGET)
