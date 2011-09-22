
CFLAGS += -D__KERNEL__ -DMODULE -DEXPORT_SYMTAB  -march=i686
obj-m := hello_world.o
KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

%.o : %.c
	gcc $(CFLAGS) $(EXTRA_CFLAGS) -c  $^

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.mod.c *.o *.ko .tmp_versions
