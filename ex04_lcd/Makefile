KERNELDIR := /home/linux/work/kernel/linux-3.14-fs4412/
#KERNELDIR := /lib/modules/$(shell uname -r)/build
obj-m := lcd.o
all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

.PHONY:clean
clean:
	rm -rvf *.ko *.o a.out Module* modules* *.mod.c .*.cmd .tmp_versions
