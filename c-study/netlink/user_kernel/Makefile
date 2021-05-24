#
#Desgin of Netlink
#

MODULE_NAME :=netlink_kernel
obj-m :=$(MODULE_NAME).o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

#CC = mips64-octeon-linux-gnu-gcc
CC = gcc
CFLAGS_LOCAL = -Wall -O2
LDFLAGS = 
TARGET = nl_user
OBJS = user.o
COMPILE  = $(CC) $(CFLAGS_LOCAL) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

USER:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f $(OBJS) *~ *.d *.o $(TARGET)

