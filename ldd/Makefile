srcfile := proc_file

ifeq ($(KERNELRELEASE),)

KERN_PATH := /lib/modules/$(shell uname -r)/build
LOCAL_PATH := $(shell pwd)

modules:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules

modules_install:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules_install

clean:
	rm -rf *.o *.ko *.mod.o *.mod.c *.order *.symvers

.PHONY: modules modules_install clean

else

obj-m := $(srcfile).o

endif

