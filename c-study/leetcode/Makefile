
CC=gcc
CFLAGS = -Wall
LFLAGS =
COMPILE = $(CC) $(CFLAGS) $< -c -o $@
LINK = $(CC) $(LFLAGS) $^ -o $@
OBJS = buildin.o
TARGET = a.out

all:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)
%.o:%.c
	$(COMPILE)
clean:
	rm -f *.o $(TARGET)
