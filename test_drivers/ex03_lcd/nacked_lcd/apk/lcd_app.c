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
	int err = -1;
	int fd_fb = open(PATHNAME, O_RDWR);
	while(err){
		err = write(fd_fb, gImage_lcd_picture, strlen(gImage_lcd_picture));
	}

	close(fd_fb);
	return 0;
}
