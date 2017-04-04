KERNELDIR := /home/linux/work/kernel/linux-3.14-fs4412/ 

obj-m += exynos4412_lcd.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) clean
