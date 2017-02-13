#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
	unsigned int* addr = NULL;
	int fd = -1;
	size_t length = 0xff;
	off_t gpx2_base = 0x11000c40;
	off_t gpx2_con_off = 0x0;
	off_t gpx2_dat_off = 0x4;
	unsigned int* led2_con = NULL;
	unsigned int* led2_dat = NULL;

	fd = open("/dev/mem", O_RDWR);

	addr = (unsigned int*)mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED,fd, gpx2_base);

	printf("%d\n",__LINE__);
	led2_con = addr + gpx2_con_off;
	led2_dat = addr + gpx2_dat_off;
	printf("%d\n",__LINE__);
	printf("addr:%x\n",*addr);
	printf("led2_con:%x, led2_dat:%x\n",*led2_con,*led2_dat);
	//led2 con
#if 0
	*(volatile unsigned char*)led2_con  = ((*(unsigned int*)led2_con) &(~(0xf<<28)))|(0x1<<28);	
	//led2_on
	*(volatile unsigned char*)led2_dat = (*(unsigned int*)led2_dat) | (0x1<<7);
	printf("%d\n",__LINE__);
	sleep(5);
	//led2_off
	*(volatile unsigned char*)led2_dat = (*(unsigned int*)led2_dat) & ~(0x1<<7);
#endif
#if 1
	printf("%d\n",__LINE__);
	*led2_con  = ((*led2_con) &(~(0xf<<28)))|(0x1<<28);	
	//led2_on
	*led2_dat = (*led2_dat) | (0x1<<7);
	sleep(5);
	//led2_off
	*led2_dat = (*led2_dat) & ~(0x1<<7);
#endif
	printf("%d\n",__LINE__);
	munmap(addr, length);
	printf("%d\n",__LINE__);
	close(fd);
	printf("%d\n",__LINE__);
	return 0;
}
