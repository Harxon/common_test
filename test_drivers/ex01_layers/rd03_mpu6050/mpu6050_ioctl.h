#ifndef __MPU6050_IOCTL__
#define __MPU6050_IOCTL__

#define MAGIC_TYPE 'M'
typedef struct MPU6050_ACCEL{
	unsigned int x;
	unsigned int y;
	unsigned int z;
}accel_t;
typedef struct MPU6050_GYRO{
	unsigned int x;
	unsigned int y;
	unsigned int z;
}gyro_t;
typedef struct MPU6050_MSG{
	accel_t accel;
	gyro_t gyro;
}mpu6050_msg_t;

#define GET_X _IOR(MAGIC_TYPE, 'X', float)
#define GET_Y _IOR(MAGIC_TYPE, 'Y', float)
#define GET_Z _IOR(MAGIC_TYPE, 'Z', float)
#define GET_ALL _IOR(MAGIC_TYPE, 'A', mpu6050_msg_t)

#endif
