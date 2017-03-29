#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/device.h>

#define MINORS_N 2

static struct class * clsp = NULL;
static struct device* decp[MINORS_N] = {};
static dev_t dev = 0;

int interface_cdev_init(void){
	int err = -1;
	int i = 0;
	err = alloc_chrdev_region(&dev, 0, MINORS_N, "alloc_chrdev_region");
	
	//create MINORS_N node
	clsp = class_create(THIS_MODULE,"harxon_cdev_class");
	if(!clsp){
		printk("class_create failed!\n");
	}else{}
	
	for(i=0; i<MINORS_N; i++){
		decp[i] = device_create(clsp, NULL, MKDEV(MAJOR(dev),MINOR(dev) + i), NULL,"ex%d", i);
	}

	printk(KERN_INFO "harxon_cdev_init success\n");
	return 0;
}

void interface_cdev_exit(void){
	int i = 0;
	for(i=0; i<MINORS_N; i++){
		device_destroy(clsp,MKDEV(MAJOR(dev),MINOR(dev) + i));
	}
	class_destroy(clsp);
	unregister_chrdev_region(dev, MINORS_N);
	printk(KERN_INFO "harxon_cdev_exit done!\n");
}

module_init(interface_cdev_init);
module_exit(interface_cdev_exit);
MODULE_LICENSE("GPL");
