#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "lcd_picture.h"

#define PATHNAME "/dev/fb0"

int main(int argc, const char *argv[])
{
	int cnt = -1;
	//char buffer[1024*600*2] = {};
	const unsigned char* p = gImage_lcd_picture;

	int fd = open(PATHNAME, O_RDWR);
	while(cnt){
		cnt = write(fd, p, strlen(p));
		printf("cnt_write:%d\n", cnt);
		p += cnt;
	}

	close(fd);
	return 0;
}
