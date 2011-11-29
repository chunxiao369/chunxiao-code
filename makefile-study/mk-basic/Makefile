TARGET := test
CC := gcc
AR := ar
OBJ_DIR := objs
OBJS := $(OBJ_DIR)/test.o
#objs in current directory
TOP := $(PWD)
CFLAGS_GLOBAL=-I$(TOP)/include -g -Wall
COMPILE  =  $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<
MAKELIB  = $(AR) -cr $@ $^
CFLAGS_LOCAL = 

#the directory where the objs put

all: $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.c
	$(COMPILE)

-include $(OBJS:.o=.d)

dir := module
include $(dir)/module.mk

dir := tb
include $(dir)/tb.mk

$(TARGET): $(OBJ_DIR) $(OBJS) $(LIBS_LIST)
	@echo '|----' $(OBJ_DIR)
	@echo '|----' $(OBJS)
	@echo '|----' $(LIBS_LIST)
	@echo '|----' $(CFLAGS_LOCAL)
	@echo '|----' $(OBJS_module)
	@echo '|----' $(OBJS_tb)
	$(CC) $(OBJS) $(LDFLAGS_PATH) $(LIBS_LIST) $(LDFLAGS_GLOBAL) -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
