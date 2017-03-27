KERNELDIR := /lib/modules/$(shell uname -r)/build
obj-m += test_kobj.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) clean
