ifeq ($(KERNELRELEASE),)
	#KERNELDIR  :=/lib/modules/$(shell  uname  -r)/build
	KERNELDIR  := /home/linux/work/kernel/linux-3.14-fs4412/
	PWD := `pwd`
all:
	make  -C  $(KERNELDIR)  M=$(PWD)   modules

clean:
	make  -C  $(KERNELDIR)  M=$(PWD)   clean
else

obj-m +=fs_lcd.o


endif
