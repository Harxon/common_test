#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/kern_levels.h>

#define __CDEV_DEBUG__
#ifdef __CDEV_DEBUG__
#define HARXON_DEBUG() printk(KERN_INFO "(%s/%s):%d\n",__FILE__, __func__, __LINE__)

#ifndef __HARXON__
#define DEBUG(KERN_EMERG,arg...) printk(KERN_EMERG,##arg)
#else
#define DEBUG(n, arg...)
#endif

#endif

struct miscdevice* miscp;

static int misc_open(struct inode* inodp, struct file* filp){

 	HARXON_DEBUG();
	return 0;
}
static int misc_release(struct inode* inodp, struct file* filp){


 	HARXON_DEBUG();
	return 0;
}

static struct file_operations misc_fops = {
		.owner = THIS_MODULE,
		.open = misc_open,
		.release = misc_release,

};

static int sub_misc_init(void){

	miscp = kzalloc(sizeof(*miscp), GFP_KERNEL);
	miscp->minor = MISC_DYNAMIC_MINOR;
	miscp->name = "ex_misc_00";
	miscp->fops = &misc_fops;
	
	misc_register(miscp);
	
 	HARXON_DEBUG();
	DEBUG("(%s/%s):%d\n",__FILE__, __func__, __LINE__);
	return 0;
}
static void sub_misc_exit(void){

	misc_deregister(miscp);
	kfree(miscp);
 	HARXON_DEBUG();
}
module_init(sub_misc_init);
module_exit(sub_misc_exit);
MODULE_LICENSE("GPL");
