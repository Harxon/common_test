#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define Debug() printf("(%s):%d\n", __func__, __LINE__); 

//define MMAP_SIZE sysconf(_SC_PAGESIZE)
#define MMAP_SIZE 4096UL
#define MMAP_MASK (MMAP_SIZE - 1)

int main(int argc, const char *argv[])
{
	int fd = -1, i=0;
	void* map_addr = NULL;
	void* vir_addr = NULL;
	void* dat_addr = NULL;
	unsigned long target[2] = {0x11000c40, 0x11000c44};
	char type[3] = "2_7";
	unsigned long opt[2] ={1<<(type[2]*4), 1<<(type[2])};

#if 0
	switch(argc){
	case 4:{
			   opt[0] = strtoul(argv[3], 0, 0);
		   }
	case 3:{
			   type[2] = tolower(argv[2][2]);
		   }
	case 2:{
			   target[0] = strtoul(argv[1], 0, 0);
		   }
	case 1:{
			   break;
		   }
	default:break;
	}
#endif

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(fd == -1){
		Debug();
		exit(-1);
	}

	map_addr = mmap(NULL, MMAP_SIZE, PROT_READ|PROT_WRITE,  MAP_SHARED, fd, target[0] & ~MMAP_MASK);
	vir_addr = map_addr + (target[0] & MMAP_MASK);
	dat_addr = vir_addr + 4;

	printf("gpx2con_vaddr:%lp, data:%#x\n", vir_addr, *(unsigned long*)vir_addr);
	printf("gpx2dat_vaddr:%lp, data:%#x\n", dat_addr, *(unsigned long*)dat_addr);
	*(unsigned long*)vir_addr &= ~(0xf<<28);
	*(unsigned long*)vir_addr |= (1<<28);
	for(i=0; i<2; i++){
		*(unsigned long*)dat_addr |= (1<<7); 
		sleep(1);
		*(unsigned long*)dat_addr &= ~(1<<7); 
		sleep(1);
	}
	printf("gpx2con_vaddr:%lp, data:%#x\n", vir_addr, *(unsigned long*)vir_addr);
	printf("gpx2dat_vaddr:%lp, data:%#x\n", dat_addr, *(unsigned long*)dat_addr);

	Debug();
	munmap(vir_addr, MMAP_SIZE);
	return 0;
}
