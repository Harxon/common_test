#KERNDIR := /home/linux/work/kernel/linux-3.14-OK/
KERNDIR := /lib/modules/3.16.0-30-generic/build/
obj-m += test_do_while.o

modules:
	$(MAKE) -C $(KERNDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KERNDIR) M=$(shell pwd) clean
	

