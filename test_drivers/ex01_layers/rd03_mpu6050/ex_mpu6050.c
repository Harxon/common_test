#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#include <asm/string.h>
#include "mpu6050_ioctl.h"
#define SMPLRT_DIV		0x19
#define CONFIG			0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define ACCEL_XOUT_H	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
#define TEMP_OUT_H		0x41
#define TEMP_OUT_L		0x42
#define GYRO_XOUT_H		0x43
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48
#define PWR_MGMT_1		0x6B



#define Debug() printk(KERN_INFO "(%s):%d\n", __func__, __LINE__)
#define Debug_err() printk(KERN_ERR"ERR!!!:(%s):%d\n", __func__, __LINE__)
static struct miscdevice * misc = NULL;
static struct i2c_client* client = NULL;



static __u8 read_reg_mpu6050(struct i2c_client* client, __u8 reg){

	__u8* rxbuf = NULL;
	struct i2c_msg msg[2] = {
		{client->addr, 0, sizeof(__u8), &reg},
		{client->addr, 1, sizeof(__u8), rxbuf},
	};
	Debug();

	if(0 > i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg))){
		Debug_err();
		return -ENOMSG;
	}else{}
	
	return *rxbuf;
}
static __u8 write_reg_mpu6050(struct i2c_client* client, __u8 reg, __u8 val){
		
	struct i2c_msg msg[2] = {
		{client->addr, 0, sizeof(__u8), &reg},
		{client->addr, 0, sizeof(__u8), &val},
	};
	Debug();

	if(0 > i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg))){
		Debug_err();
		return -ENOMSG;
	}else{}

	return 0;	
}

static int mpu6050_init(struct i2c_client* clientp){
	Debug();
	write_reg_mpu6050(clientp, PWR_MGMT_1, 0x00);
	write_reg_mpu6050(clientp, SMPLRT_DIV, 0x07);
	write_reg_mpu6050(clientp, CONFIG, 0x06);
	write_reg_mpu6050(clientp, GYRO_CONFIG, 0xF8);
	write_reg_mpu6050(clientp, ACCEL_CONFIG, 0x19);

	return 0;
}

static long mpu6050_ioctl(struct file* filp, unsigned int cmd, unsigned long arg){
	Debug();
	if(MAGIC_TYPE != _IOC_TYPE(cmd)){
		Debug_err();
		return -EINVAL;
	}else{}
	switch(_IOC_NR(cmd)){
		case 'X':{
			arg = 0;
			arg = read_reg_mpu6050(client, GYRO_XOUT_H) << 8;
			arg |= read_reg_mpu6050(client, GYRO_XOUT_L) << 8;
			break;
		}
		case 'Y':{
			
			break;
		}
		case 'Z':{
			
			break;
		}
		default:{
			
			break;
		}

	}

	return 0;
}

static const struct file_operations mpu6050_fops={
	.owner = THIS_MODULE,
	.unlocked_ioctl = mpu6050_ioctl,
};

/* Standard driver model interfaces */
static int mpu6050_i2c_probe(struct i2c_client* clientp, const struct i2c_device_id* idp){
	Debug();
	misc = kzalloc(sizeof(struct miscdevice), GFP_KERNEL);
	if(!misc){
		Debug_err();
		goto err_kzalloc;
	}else{
		misc->name = "mpu6050";
		misc->minor = MISC_DYNAMIC_MINOR;
		misc->fops = &mpu6050_fops;
	}
	if(misc_register(misc)){
		Debug_err();
		goto err_misc_register;
	}else{
		client = clientp;
	}

	mpu6050_init(clientp);

	return 0;
	
err_misc_register:
	kfree(misc);
err_kzalloc:
	return 0;
}
int mpu6050_i2c_remove(struct i2c_client * clientp){
	Debug();
	
	misc_deregister(misc);
	kfree(misc);

	return 0;
}

static const struct of_device_id mpu6050_dt_ids[] = {
	{ .compatible = "xyz,mpu6050", },
	{ }
};
static const struct i2c_device_id mpu6050_i2c_id[] = {
	{ "mpu6050-exynos4412", 0 },
	{ }
};
static struct i2c_driver mpu6050_i2c_driver = {
	.driver = {
		.name	= "mpu6050",
		.owner	= THIS_MODULE,
		.of_match_table	= of_match_ptr(mpu6050_dt_ids),
	},
	.probe		= mpu6050_i2c_probe,
	.remove		= mpu6050_i2c_remove,
	.id_table	= mpu6050_i2c_id,
};
module_i2c_driver(mpu6050_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Handsome");
MODULE_DESCRIPTION("mpu6050 driver");
