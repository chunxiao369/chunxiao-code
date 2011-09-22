TARGET1 := client
TARGET2 := server
CC := gcc
AR := ar
OBJS1 := socket-client.o
OBJS2 := socket-server.o
TOP := $(PWD)
CFLAGS_GLOBAL= -g -Wall
COMPILE  =  $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<
MAKELIB  = $(AR) -cr $@ $^
CFLAGS_LOCAL = 

all: $(TARGET1) $(TARGET2)

%.o : %.c
	$(COMPILE)

-include $(OBJS:.o=.d)

$(TARGET1): $(OBJS1) 
	$(CC) $< $(LDFLAGS_PATH) $(LIBS_LIST) $(LDFLAGS_GLOBAL) -o $@

$(TARGET2): $(OBJS2) 
	$(CC) $< $(LDFLAGS_PATH) $(LIBS_LIST) $(LDFLAGS_GLOBAL) -o $@

clean:
	rm -rf $(TARGET1) $(TARGET2) *.o *.d
