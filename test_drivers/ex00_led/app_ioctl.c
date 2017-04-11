#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "fs4412_ioctl.h"

#define FILENAME "/dev/led2"

int main(int argc, const char *argv[])
{
	int fd = -1;
	fd = open(FILENAME,O_RDWR);
	if(fd < 0){
		perror("open failed");
		return -1;
	}
	ioctl(fd,LED_ON);
	sleep(1);
	ioctl(fd,LED_OFF);
	close(fd);
	return 0;
}
