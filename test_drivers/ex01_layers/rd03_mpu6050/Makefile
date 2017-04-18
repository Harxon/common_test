KERNDIR := /home/linux/work/kernel/linux-3.14-OK/
obj-m += ex_mpu6050.o

modules:
	$(MAKE) -C $(KERNDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KERNDIR) M=$(shell pwd) clean
	

