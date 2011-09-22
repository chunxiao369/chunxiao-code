TARGET := test
CC := gcc
AR := ar
OBJS := cli_simple.o
INCLUDE_DIR :=
CFLAGS_GLOBAL = -g -Wall -I$(INCLUDE_DIR) -g -Wall
COMPILE  =  $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<
MAKELIB  = $(AR) -cr $@ $^
CFLAGS_LOCAL = 

#the directory where the objs put

all: $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

%.o : %.c
	$(COMPILE)

-include $(OBJS:.o=.d)

$(TARGET):  $(OBJS) $(LIBS_LIST)
	$(CC) $(OBJS) $(LDFLAGS_PATH) $(LIBS_LIST) $(LDFLAGS_GLOBAL) -o $@

clean:
	rm -rf  $(TARGET) $(OBJS)
