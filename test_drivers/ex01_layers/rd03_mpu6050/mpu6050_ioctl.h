#ifndef __MPU6050_IOCTL__
#define __MPU6050_IOCTL__

#define MAGIC_TYPE 'M'
#define GET_X _IOR(MAGIC_TYPE, 'X', float)
#define GET_Y _IOR(MAGIC_TYPE, 'Y', float)
#define GET_Z _IOR(MAGIC_TYPE, 'Z', float)
#endif