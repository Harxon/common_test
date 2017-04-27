/* Includes */
#include <linux/module.h>		/* For module specific items */
#include <linux/moduleparam.h>		/* For new moduleparam's */
#include <linux/types.h>		/* For standard types (like size_t) */
#include <linux/errno.h>		/* For the -ENODEV/... values */
#include <linux/kernel.h>		/* For printk/panic/... */
#include <linux/miscdevice.h>		/* For struct miscdevice */
#include <linux/watchdog.h>		/* For the watchdog specific items */
#include <linux/fs.h>			/* For file operations */
#include <linux/ioport.h>		/* For io-port access */
#include <linux/platform_device.h>	/* For platform_driver framework */
#include <linux/init.h>			/* For __init/__exit/... */
#include <linux/uaccess.h>		/* For copy_to_user/put_user/... */
#include <linux/io.h>			/* For inb/outb/... */
#include <linux/of.h>
#include <linux/delay.h>

/* Module information */
#define DRV_NAME "harxonwdt"
#define WATCHDOG_NAME "Harxon WDT"
/* There is no way to see what the correct time-out period is */
#define WATCHDOG_HEARTBEAT 0

unsigned int* wdt_vddr = NULL;
unsigned int* wtcon = NULL;
unsigned int* wtdat = NULL;
unsigned int* wtcnt = NULL;
unsigned int* wtclrint = NULL;
typedef struct {
	unsigned int con;
	unsigned int dat;
	unsigned int cnt;
	unsigned clrint;
}wdt_info_t;

//#define pr_info(arg) printk(KERN_INFO ##arg)

/*
 *	Kernel Interfaces
 */

static long exy_wdt_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	long ret = 0;
	wdt_info_t info;
	switch(_IOC_NR(cmd)){
		case 'a':{
			info.con = readl(wtcon);
			info.dat = readl(wtdat);
			info.cnt = readl(wtcnt);
			if(0 > (ret = copy_to_user((void*)arg, &info, sizeof(wdt_info_t)))){
				printk(KERN_ERR "copy_to_user failed\n");
				return -EAGAIN;
			}
			break;
		}
		default:break;
	}
	return 0;
}

static const struct file_operations exy_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = exy_wdt_ioctl,
};

static struct miscdevice exy_miscdev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "exy_wdt",
	.fops	= &exy_fops,
};

/*
 *	Init & exit routines
 */

static int exy_probe(struct platform_device *dev)
{
	int ret;
	struct resource* res = NULL;
	res = platform_get_resource(dev,IORESOURCE_MEM,0);
	printk("start:%#x\n", res->start);
	wdt_vddr = ioremap(res->start, 4*4);
	wtcon = wdt_vddr + 0;
	wtdat = wdt_vddr + 1;
	wtcnt = wdt_vddr + 2;
	wtclrint= wdt_vddr + 3;

	printk("________________________________\n");
	printk("wtcon:%#x\n",readl(wtcon));
	printk("wtdat:%#x\n",readl(wtdat));
	printk("wtcnt:%#x\n",readl(wtcnt));
	printk("wtclrint:%#x\n",readl(wtclrint));
	printk("________________________________\n");

#if 0
//开门狗寄存器只有在开门狗功能使能位5th被打开的情况下才能操作该寄存器
	writel(((((readl(wtcon)
				| (0xffff<<8))
				| (0x1<<5)) 
				| (0x1<<3))
				| (0x1<<2))
				& (~(0x1<<0)), (void*)wtcon);
#endif
#if 1
//开门狗寄存器只有在开门狗功能使能位5th被打开的情况下才能操作该寄存器
		writel(((((((readl(wtcon)
				| (0x8<<8))
				& (~(0x1<<5))) 
				& (~(0x11<<3)))
				| (0x01<<3))
				& (~(0x1<<2)))
				| (0x1<<0)), (void*)wtcon);
	printk("________________________________\n");
	printk("wtcon:%#x\n",readl(wtcon));
	printk("wtdat:%#x\n",readl(wtdat));
	printk("wtcnt:%#x\n",readl(wtcnt));
	printk("wtclrint:%#x\n",readl(wtclrint));
	printk("________________________________\n");
#endif
		msleep(20);
		writel(((readl(wtdat)
				& (~(0xffff<<0)))
				| (0x888<<0)), (void*)wtdat);	
		msleep(20);
		writel(((readl(wtcnt)
				& (~(0xffff<<0)))
				| (0x333<<0)), (void*)wtcnt);
	printk("________________________________\n");
	printk("wtcon:%#x\n",readl(wtcon));
	printk("wtdat:%#x\n",readl(wtdat));
	printk("wtcnt:%#x\n",readl(wtcnt));
	printk("wtclrint:%#x\n",readl(wtclrint));
	printk("________________________________\n");
#if 1
		msleep(20);
		writel((readl(wtcon)
					| (0x1<<5)) , (void*)wtcon);
#endif

	printk("________________________________\n");
	printk("wtcon:%#x\n",readl(wtcon));
	printk("wtdat:%#x\n",readl(wtdat));
	printk("wtcnt:%#x\n",readl(wtcnt));
	printk("wtclrint:%#x\n",readl(wtclrint));
	printk("________________________________\n");
	ret = misc_register(&exy_miscdev);
	if (ret != 0) {
		printk(KERN_ERR "cannot register miscdev on minor=%d (err=%d)\n",
		       exy_miscdev.minor, ret);
		goto un_ioremap;
	}
	printk("initialized. \n");

	return 0;
un_ioremap:
	iounmap(wdt_vddr);
	return ret;
}


static int exy_remove(struct platform_device *dev)
{
	misc_deregister(&exy_miscdev);
	iounmap(wdt_vddr);
	return 0;
}

static void exy_shutdown(struct platform_device *dev)
{
	/* Turn the WDT off if we have a soft shutdown */
}
static struct of_device_id ids[] = {
	{.compatible = "exynos4412,wdt",},
	{},
};
static struct platform_driver exywdt_driver = {
	.probe		= exy_probe,
	.remove		= exy_remove,
	.shutdown	= exy_shutdown,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= DRV_NAME,
		.of_match_table = of_match_ptr(ids),
	},
};

static int __init exy_init(void)
{
	int err;

	printk("WDT driver for exy single board computer initialising\n");

	err = platform_driver_register(&exywdt_driver);
	if (err)
		return err;

	return 0;
}

static void __exit exy_exit(void)
{
	platform_driver_unregister(&exywdt_driver);
	printk(KERN_INFO "Watchdog Module Unloaded\n");
}

module_init(exy_init);
module_exit(exy_exit);

MODULE_AUTHOR("Harxon Hao");
MODULE_DESCRIPTION("Exynos4412 Single Board Computer Watchdog Timer driver");
MODULE_LICENSE("GPL");
