#KVER = /usr/src/linux   
KVER = /lib/modules/`uname -r`/build  
CURDIR = $(shell pwd)  
# Kernel modules   
obj-m := pl2303.o  
build: kernel_modules  
kernel_modules:  
	$(MAKE) -C $(KVER) M=$(CURDIR) modules  
clean:  
	$(MAKE) -C $(KVER) M=$(CURDIR) clean  
