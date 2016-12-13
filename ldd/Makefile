ifeq ($(KERNELRELEASE),)

KERN_PATH := /lib/modules/$(shell uname -r)/build
LOCAL_PATH := $(shell pwd)
demo_path := demo

modules:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules
	$(MAKE) -C $(demo_path)
#	cp $(sf).ko ~/

modules_install:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules_install

clean:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) clean
	$(MAKE) -C $(demo_path) clean

install:
	cp $(sf).ko ~/

.PHONY: modules modules_install clean

else

# obj-m += bitops/
obj-m += chrdev/
#obj-m += timer/
obj-m += tasklet/
obj-m += workqueue/
obj-m += memory/
obj-m += devmod/

endif

