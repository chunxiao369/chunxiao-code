
obj-m := hello_world.o
KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.mod.c *.o *.ko .tmp_versions
