ifeq ($(KERNELRELEASE),)

KERN_PATH := /lib/modules/$(shell uname -r)/build
LOCAL_PATH := $(shell pwd)

modules:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules
#	cp $(sf).ko ~/

modules_install:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules_install

clean:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) clean

install:
	cp $(sf).ko ~/

.PHONY: modules modules_install clean

else

# obj-m += bitops/
obj-m += chrdev/
obj-m += timer/
obj-m += tasklet/
obj-m += workqueue/
obj-m += memory/

endif

