ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /home/linux/work/kernel/linux-3.0-fs4412_V3/
#KERNELDIR ?= /home/linux/work/kernel/linux-3.14-OK/
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean 

else
    obj-m := led2_drv.o
endif

