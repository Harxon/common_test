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
	int ret = -1;
	unsigned long gpx2con_phy_addr = 0;
	unsigned long* vaddr_base = NULL;
	unsigned long* gpx2con_vaddr = NULL;
	unsigned long* gpx2dat_vaddr = NULL;
	unsigned long regcon_data;
	unsigned long regdat_data;

	if(argc<4){
		printf("Please input: %s + address + condata + datdata");
		return -1;
	}

	gpx2con_phy_addr = strtoul(argv[1], 0, 0);
	regcon_data = strtoul(argv[2], 0, 0);
	regdat_data = strtoul(argv[3], 0, 0);
	fd_mem = open("/dev/mem", O_RDWR | O_SYNC);
	{
		Debug();
		vaddr_base = mmap(0, 4096, PROT_READ|PROT_WRITE,  MAP_SHARED, fd_mem, gpx2con_phy_addr & ~(4096-1));
		gpx2con_vaddr = vaddr_base + (gpx2con_phy_addr & (4096 -1));
		gpx2dat_vaddr = gpx2con_vaddr + 1;
		Debug();
		printf("%lp\n", gpx2con_vaddr);
		printf("%p\n", gpx2con_vaddr+1);
		Debug();
		*gpx2con_vaddr |= regcon_data;
		Debug();
		*gpx2dat_vaddr |= regdat_data; 
	}
	Debug();
	munmap(gpx2con_vaddr, 4096);

	getchar();
	return 0;
}
