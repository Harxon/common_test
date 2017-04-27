#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
typedef struct {
	unsigned int con;
	unsigned int dat;
	unsigned int cnt;
	unsigned clrint;
}wdt_info_t;
#define MAGIC_TYPE 'W'
#define GET_ALL _IOR(MAGIC_TYPE, 'a', sizeof(wdt_info_t))
int main(int argc, const char *argv[])
{
	int fd = -1;
	wdt_info_t info;
	fd = open("/dev/exy_wdt", O_RDWR);
	while(1){
		ioctl(fd, GET_ALL, &info);
		printf("cnt:%d\n",info.cnt);
		sleep(1);
	}
	return 0;
}
