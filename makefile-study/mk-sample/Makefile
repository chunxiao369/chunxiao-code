TARGET := test
CC := gcc
AR := ar
OBJ_DIR := objs
OBJS := $(OBJ_DIR)/test.o
#objs in current directory
TOP := $(PWD)
CFLAGS_GLOBAL=-g -Wall -Werror
MAKELIB  = $(AR) -cr $@ $^
CFLAGS_LOCAL := -I$(TOP)/moduleA -I$(TOP)/moduleB
LDFLAGS_PATH = -L$(OBJ_DIR)
COMPILE  =  $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<
#the directory where the objs put

all: $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.c
	$(COMPILE)

-include $(OBJS:.o=.d)

dir := moduleA
include $(dir)/moduleA.mk

dir := moduleB
include $(dir)/moduleB.mk

$(TARGET): $(OBJ_DIR) $(OBJS) $(LIBS_LIST)
	@#$(CC) $(OBJS) $(LDFLAGS_PATH) $(LIBS_LIST) $(LDFLAGS_GLOBAL) -o $@
	$(CC) $(OBJS) $(LDFLAGS_PATH) $(LDFLAGS_GLOBAL) -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
