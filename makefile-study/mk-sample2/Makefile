TARGET := test
CC := gcc
AR := ar
TOP := $(PWD)
OBJ_DIR := objs
KOBJ_DIR := kobjs
OBJS := $(OBJ_DIR)/test.o \
        $(OBJ_DIR)/moduleA.o \
        $(OBJ_DIR)/moduleB.o

#objs in current directory
CFLAGS_GLOBAL=-I$(TOP)/include -g -Wall
COMPILE  =  $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -O -c -o $@ $<
MAKELIB  = $(AR) -cr $@ $^
CFLAGS_LOCAL = 
LDFLAGS_PATH = -L$(OBJ_DIR)
#the directory where the objs put

all: $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.c
	$(COMPILE)
$(OBJ_DIR)/%.o : moduleA/%.c
	$(COMPILE)
$(OBJ_DIR)/%.o : moduleB/%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

$(TARGET): $(OBJ_DIR) $(OBJS) $(LIBS_LIST)
	$(CC) $(OBJS) $(LDFLAGS_PATH) $(LDFLAGS_GLOBAL) -o $@

$(KOBJ_DIR):
	mkdir $(KOBJ_DIR)
obj-m := hello_world.o
$(TOP)/kobjs/hello.o : $(TOP)/hello.c
	$(call cmd,force_checksrc)
	$(call if_changed_rule,cc_o_c)

$(TOP)/kobjs/moduleA.o : $(TOP)/moduleA/moduleA.c
	$(call cmd,force_checksrc)
	$(call if_changed_rule,cc_o_c)

$(TOP)/kobjs/moduleB.o : $(TOP)/moduleB/moduleB.c
	$(call cmd,force_checksrc)
	$(call if_changed_rule,cc_o_c)

hello_world-objs := $(KOBJ_DIR)/hello.o
hello_world-objs += $(KOBJ_DIR)/moduleA.o
hello_world-objs += $(KOBJ_DIR)/moduleB.o

KERNELDIR = /lib/modules/$(shell uname -r)/build
kernel:$(KOBJ_DIR)
	$(MAKE) -C $(KERNELDIR) M=$(TOP) modules
kclean:
	rm -fr $(KOBJ_DIR)  *.ko Module.symvers *.mod.c *.o modules.order

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
