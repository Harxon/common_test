#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define Debug() printf("(%s):%d\n", __func__, __LINE__); 


int main(int argc, const char *argv[])
{
	int fd_mem = -1;
	unsigned long gpx2con_phy_addr = 0;
	unsigned long* vaddr_base = NULL;
	unsigned long* gpx2con_vaddr = NULL;
	unsigned long* gpx2dat_vaddr = NULL;

	gpx2con_phy_addr = strtoul("0x11000c44", 0, 0);

	fd_mem = open("/dev/mem", O_RDWR | O_SYNC);
	{
		Debug();
		vaddr_base = mmap(0, 4096UL, PROT_READ|PROT_WRITE,  MAP_SHARED, fd_mem, gpx2con_phy_addr & ~(4096UL - 1));
		gpx2con_vaddr = vaddr_base + (gpx2con_phy_addr & (4096UL -1));
		*gpx2con_vaddr = (1<<7);
		printf("%lp:%#x\n", gpx2con_vaddr, *gpx2con_vaddr);
#if 0
		gpx2dat_vaddr = gpx2con_vaddr + 1;
		printf("%p:%#x\n", gpx2dat_vaddr, *gpx2dat_vaddr);
		*gpx2dat_vaddr = (0<<7); 
		sleep(1);
		*gpx2dat_vaddr = (1<<7); 
#endif
	}
	Debug();
	munmap(gpx2con_vaddr, 8);

	getchar();
	return 0;
}
