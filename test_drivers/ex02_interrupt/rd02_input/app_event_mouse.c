#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define DEV_MOUSE "/dev/input/event3"
#define DEV_KEYBOARD "/dev/input/event1"

typedef struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	int value;
}data_t;

void show_data(data_t* data){
	printf("type:%u\ncode:%u\nvalue:%d\n",data->type, data->code, data->value);
}
int main(int argc, const char *argv[])
{
	int fd = -1;
	int cnt = -1;
	data_t data;
	if(0 > (fd = open(DEV_MOUSE, O_RDWR))){
		perror("open event1 failed");
		return -1;
	}else{}
	
	while(1){
		bzero(&data, sizeof(data_t));
		cnt = read(fd, &data, sizeof(data_t));
		if(cnt < 0){
			perror("read failed");
			close(fd);
			return -1;
		}
		show_data(&data);
		sleep(1);
		
	}
	close(fd);
	return 0;
}
