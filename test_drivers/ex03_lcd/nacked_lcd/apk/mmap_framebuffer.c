#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <string.h>
#include "lcd_picture.h"


#define MMAP_LENGTH 1024*600
#define Debug() printf("(%s):%d\n", __func__, __LINE__); 

//define MMAP_SIZE sysconf(_SC_PAGESIZE)

static int get_fb_info(int fd, struct fb_var_screeninfo* var, struct fb_fix_screeninfo* fix){

	if(ioctl(fd, FBIOGET_FSCREENINFO, fix)){
		perror("ioctl FBIOGET_FSCREENINFO failed");
		return -1;
	}

	if(ioctl(fd, FBIOGET_VSCREENINFO, var)){
		perror("ioctl VBIOGET_FSCREENINFO failed");
		return -1;
	}

	return 0;
}

int main(int argc, const char *argv[])
{
	int fd = -1;
	int cnt = -1;
	char p = 0;
	void* map_addr = NULL;
	char buf[MMAP_LENGTH] = {};
	struct fb_var_screeninfo var = {};	/* Current var */
	struct fb_fix_screeninfo fix = {};	/* Current fix */
	int red = 225;
	
	memcpy(buf, gImage_lcd_picture, MMAP_LENGTH);

	fd = open("/dev/fb0", O_RDWR | O_SYNC);
	if(fd == -1){
		Debug();
		exit(-1);
	}


	if(get_fb_info(fd, &var, &fix)){
		perror("get_fb_info failed");
		goto err_get_fb_info;
	}
	printf("xres:%d\n", var.xres);
	printf("yres:%d\n", var.yres);
	printf("bits_per_pixel:%d\n", var.bits_per_pixel);
	
	for(cnt=0; cnt<MMAP_LENGTH*16/8; cnt++){
		*(char*)(fd+p+0) = red;
		*(char*)(fd+p+1) = 0;
		*(char*)(fd+p+2) = 0;
		p +=3;
	}
#if 0
	map_addr = mmap(NULL, MMAP_LENGTH, PROT_READ|PROT_WRITE,  MAP_SHARED, fd, 0);
	cnt = write(fd, buf, MMAP_LENGTH);
	if(-1 == cnt){
		perror("write failed");
		exit(-1);
	}else{}

	Debug();
	sleep(4);
	memset(map_addr, 0, MMAP_LENGTH);	//root
#endif
	Debug();
	munmap(map_addr, MMAP_LENGTH);
	close(fd);
	return 0;

err_get_fb_info:
	munmap(map_addr, MMAP_LENGTH);
	return 0;
}
