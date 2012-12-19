
obj-m := hello_world2.o
KERNELDIR = /lib/modules/$(shell uname -r)/build
KBUILD_EXTRA_SYMBOLS += $(PWD)/../hello_world/Module.symvers
#KBUILD_EXTRA_SYMBOLS += ../hello_world/Module.symvers
export KBUILD_EXTRA_SYMBOLS
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o Module.symvers *.mod.c
