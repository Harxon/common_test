KERNELDIR := /home/linux/work/kernel/linux-3.14-OK/

obj-m += exynos4412_lcd.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) clean
