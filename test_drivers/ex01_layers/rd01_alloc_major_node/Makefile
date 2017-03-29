KERNELDIR := /lib/modules/$(shell uname -r)/build/
obj-m += alloc_major_and_node.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) clean
