ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /home/linux/work/kernel/linux-3.14-fs4412/
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

mall:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) 
	cp *.ko /source/rootfs

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* modules*

.PHONY: modules modules_install clean

else
    obj-m := mpu6050.o
endif
