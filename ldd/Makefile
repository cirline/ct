sf := bitops

ifeq ($(KERNELRELEASE),)

KERN_PATH := /lib/modules/$(shell uname -r)/build
LOCAL_PATH := $(shell pwd)

modules:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules
	cp $(sf).ko ~/

modules_install:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules_install

clean:
	rm -rf *.o *.ko *.mod.o *.mod.c *.order *.symvers *.orig .*.o.cmd .*.ko.cmd

install:
	cp $(sf).ko ~/

.PHONY: modules modules_install clean

else

obj-m := $(sf).o

endif

