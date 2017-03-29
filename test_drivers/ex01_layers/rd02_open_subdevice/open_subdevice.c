#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/slab.h>

#define MINORS_N 2

typedef struct {
	struct cdev cdev;
	struct file_operations* fops;
}scull_t;

int ex01_open(struct inode* inodp, struct file* filp){

	printk(KERN_INFO "ex01_open success!\n");
	return 0;
}
int ex01_release(struct inode* inodp, struct file* filp){

	printk(KERN_INFO "ex01_release done!\n");
	return 0;
}
int ex02_open(struct inode* inodp, struct file* filp){

	printk(KERN_INFO "ex02_open success!\n");
	return 0;
}
int ex02_release(struct inode* inodp, struct file* filp){

	printk(KERN_INFO "ex02_release done!\n");
	return 0;
}
static struct file_operations methods[] = {
	{
		.open = ex01_open,
		.release = ex01_release,
	},
	{
		.open = ex02_open,
		.release = ex02_release,
	},
};

int base_open(struct inode* inodp, struct file* filp){
	
	printk(KERN_INFO "base_open success!\n");
	return 0;
}
int base_release(struct inode* inodp, struct file* filp){
	
	printk(KERN_INFO "base_release done!\n");
	return 0;
}
static struct file_operations base_fops = {
	.open = base_open,
	.release = base_release,
};

static struct class * clsp = NULL;
static struct device* decp[MINORS_N] = {};
static dev_t dev = 0;
scull_t* scull[MINORS_N] = {};

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
	
	//cdev init_and_add
	scull[0] = kzalloc(sizeof(scull_t), GFP_ATOMIC);
	cdev_init(&scull[0]->cdev, &base_fops);
	cdev_add(&scull[0]->cdev, dev, 1);

	printk(KERN_INFO "harxon_cdev_init success\n");
	return 0;
}

void interface_cdev_exit(void){
	int i = 0;
	
	cdev_del(&scull[0]->cdev);
	kfree(scull[0]);
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
