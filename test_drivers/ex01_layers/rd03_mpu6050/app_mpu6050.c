#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "mpu6050_ioctl.h"

void show_mpu6050_msg(mpu6050_msg_t* data){
	printf("accel:\n\tx:%u, y:%u, z:%u\n",data->accel.x, data->accel.y, data->accel.z);
	printf("gyro:\n\tx:%u, y:%u, z:%u\n", data->gyro.x, data->gyro.y, data->gyro.z);
}
int main(int argc, const char *argv[])
{
	int fd = -1;
	mpu6050_msg_t data;
	fd = open("/dev/mpu6050", O_RDWR);
	if(fd < 0){
		perror("open failed");
		return -1;
	}else{}
	
	while(1){
		if(0 > ioctl(fd, GET_ALL, &data)){
			perror("ioctl failed");	
			close(fd);
			return -1;
		}else{}
		printf("dada:%#x\n", &data);
		show_mpu6050_msg(&data);
		sleep(1);
	}
	close(fd);
	return 0;
}
