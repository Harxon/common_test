CC = arm-none-linux-gnueabi-gcc
CC = gcc

all:show1 show2
show1:mmap_framebuffer.c
	$(CC) $^ -o $@ -Wall -O2
	cp show1 /source/rootfs/ -iv  
show2:write_framebuffer_image.c
	$(CC) $^ -o $@ -Wall -O2
	cp show2 /source/rootfs/ -iv  

clean:
	rm -rf show2 show1
