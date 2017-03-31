KERNELDIR := /lib/modules/$(shell uname -r)/build/
#KERNELDIR := /home/linux/work/kernel/linux-3.14-fs4412/
obj-m += rd01_input_button.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) clean
