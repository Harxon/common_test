#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/io.h>

#define __CDEV_DEBUG__
#ifdef __CDEV_DEBUG__
#define HARXON_DEBUG() printk(KERN_INFO "(%s):%d\n",__func__, __LINE__)

#ifndef __HARXON__
#define DEBUG(KERN_EMERG,arg...) printk(KERN_EMERG,##arg)
#else
#define DEBUG(n, arg...)
#endif

#endif


static struct input_dev *button_dev;



static irqreturn_t button_interrupt(int irq, void *dummy)
{
	//input_report_key(button_dev, BTN_0, inb(BUTTON_PORT) & 1);	
	input_report_key(button_dev, BTN_0, 1);
	input_sync(button_dev);


	DEBUG("irq:%d\n", irq);
	HARXON_DEBUG();
	return IRQ_HANDLED;
}

static const struct of_device_id ofdi[] = {
	{
		.compatible = "fs4412_key",
	},
	{},
};

int key2_interrupt_probe(struct platform_device* pdevp){

	struct resource* res = NULL;
	res = platform_get_resource(pdevp, IORESOURCE_IRQ, 0);
	if (request_irq(res->start, button_interrupt, 0, "button", NULL)) {
        printk(KERN_ERR "button.c: Can't allocate irq %ud\n", res->start);
        return -EBUSY;
    }
    
	DEBUG("irqnum:%d\n", res->start);

	HARXON_DEBUG();
	return 0;
}
int key2_interrupt_remove(struct platform_device * pdevp){
	struct resource* res = NULL;
	res = platform_get_resource(pdevp, IORESOURCE_IRQ, 0);

	free_irq(res->start, button_interrupt);
	HARXON_DEBUG();
	return 0;
}

struct platform_driver pfd = {
	.probe = key2_interrupt_probe,
	.remove = key2_interrupt_remove,
	.driver.of_match_table = of_match_ptr(ofdi),
};

static int __init button_init(void)
{
	int error;

	button_dev = input_allocate_device();
	if(!button_dev){
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		return error;
	}

	button_dev->evbit[0] = BIT_MASK(EV_KEY);
	button_dev->keybit[BIT_WORD(BTN_0)] = BIT_MASK(BTN_0);

	error = input_register_device(button_dev);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev;
	}

	platform_driver_register(&pfd);

	HARXON_DEBUG();
	return 0;

 err_free_dev:
	input_free_device(button_dev);
	return error;
}

static void __exit button_exit(void)
{
	platform_driver_unregister(&pfd);
    input_unregister_device(button_dev);
	HARXON_DEBUG();
}

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");
