#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/io.h>
#include "fs4412_ioctl.h"
#include <linux/platform_device.h>


#define GPX2_7CON_ADDR 0x11000c40
#define Debug() printk("(%s):%d\n", __func__, __LINE__);
dev_t devno;
struct cdev cdev;
struct class* class;


void __iomem* led2_conaddr;
void __iomem* led2_dataddr;
	

static int fs4412_driver_open(struct inode* inode, struct file* filp){

	
	printk("fs4412_driver_open\n");
	return 0;
}
static int fs4412_driver_release(struct inode* inode, struct file* filp){
	
	printk("fs4412_driver_release\n");
	return 0;
}

long fs4412_driver_ioctl(struct file* filp, unsigned int cmd, unsigned long args){
	
	printk("type:%d, nr:%d\n",_IOC_TYPE(cmd), _IOC_NR(cmd));
	switch(_IOC_NR(cmd)){
		case 0:
		{
			writel(readl(led2_dataddr)  | (0x1 << 7), led2_dataddr);
			printk("LED ON :)\n");
			break;
		}
		case 1:
		{

			writel(readl(led2_dataddr)  & (~(0x1 << 7)), led2_dataddr);
			printk("LED OFF :)\n");
			break;
		}
		default:
			printk("you are smart~\n");
			break;
		
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = fs4412_driver_open,
	.release = fs4412_driver_release,
	.unlocked_ioctl = fs4412_driver_ioctl,
};
static int led2_con_enable(void __iomem* led2_conaddr){
	writel((readl(led2_conaddr) & (~(0xf << 28))) | (0x1 << 28),led2_conaddr);
	return 0;
}
static int led2_init(void)
{
	Debug();
	alloc_chrdev_region(&devno,0,1,"fs4412");
	cdev_init(&cdev,&fops);
	cdev_add(&cdev,devno,1);

	class = class_create(THIS_MODULE,"led2_class");
	device_create(class,NULL,devno,NULL,"led2");


	led2_conaddr = ioremap(GPX2_7CON_ADDR, 8);
	led2_dataddr = led2_conaddr + 4;

	led2_con_enable(led2_conaddr);
	Debug();
	return 0;
}

static void led2_exit(void)
{
	device_destroy(class, devno);
	class_destroy(class);
	Debug();
	iounmap(led2_conaddr);
	cdev_del(&cdev);
	unregister_chrdev_region(devno,1);
	Debug();
}


module_init(led2_init);
module_exit(led2_exit);
MODULE_LICENSE("Dual BSD/GPL");
