
CC=gcc
CFLAGS = -Wall
LFLAGS =
COMPILE = $(CC) $(CFLAGS) $< -c -o $@
LINK = $(CC) $(LFLAGS) $^ -o $@
OBJS = g.o
TARGET = xuexe

all:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)
%.o:%.c
	$(COMPILE)
clean:
	rm *.o *.d $(TARGET)
