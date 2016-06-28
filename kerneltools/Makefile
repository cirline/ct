ifeq ($(KERNELRELEASE),)

KERN_PATH := /lib/modules/$(shell uname -r)/build
LOCAL_PATH := $(shell pwd)
#demo_path := demo

modules:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules
	#$(MAKE) -C $(demo_path)

modules_install:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) modules_install

clean:
	$(MAKE) -C $(KERN_PATH) M=$(LOCAL_PATH) clean
	#$(MAKE) -C $(demo_path) clean

.PHONY: modules modules_install clean

else

obj-m += waitqueue_and_timer/

endif

