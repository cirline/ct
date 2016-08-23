ifeq ($(KERNELRELEASE),)

KERN_PATH := /lib/modules/$(shell uname -r)/build
LOCAL_PATH := $(shell pwd)

modules:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules

modules_install:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules_install

clean:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) clean

.PHONY: modules modules_install clean

else

obj-m += power/
obj-m += fs/

endif

