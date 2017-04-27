KERNDIR := /home/linux/work/kernel/linux-3.14-OK/
obj-m += watchdog_reset_sys.o

modules:
	$(MAKE) -C $(KERNDIR) M=$(shell pwd) modules
	cp *.ko /source/rootfs

clean:
	$(MAKE) -C $(KERNDIR) M=$(shell pwd) clean
	

