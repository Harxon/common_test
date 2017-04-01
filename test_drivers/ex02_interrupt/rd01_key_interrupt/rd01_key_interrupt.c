#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

#define __CDEV_DEBUG__
#ifdef __CDEV_DEBUG__
#define HARXON_DEBUG() printk(KERN_INFO "(%s):%d\n",__func__, __LINE__)

#ifndef __HARXON__
#define DEBUG(KERN_EMERG,arg...) printk(KERN_EMERG,##arg)
#else
#define DEBUG(n, arg...)
#endif

#endif


static irqreturn_t button_interrupt(int irq, void *dummy)
{
	struct resource* res = (struct resource*)dummy;
	HARXON_DEBUG();
	DEBUG("irq:%d, dummy->end:%d\n", irq, res->end);
	return IRQ_HANDLED;
}

int key2_interrupt_probe(struct platform_device* pdevp){

	struct resource* res[3] = {};
	res[0] = platform_get_resource(pdevp, IORESOURCE_IRQ, 0);
	res[1] = platform_get_resource(pdevp, IORESOURCE_IRQ, 1);
	res[2] = platform_get_resource(pdevp, IORESOURCE_IRQ, 2);
	
	HARXON_DEBUG();
	DEBUG("get_k2>> irq:%ld, flags:%u\n", res[0]->start, res[0]->flags);
	DEBUG("get_k3>> irq:%ld, flags:%u\n", res[1]->start, res[1]->flags);
	DEBUG("get_k4>> irq:%ld, flags:%u\n", res[2]->start, res[2]->flags);

	if (request_irq(res[0]->start, button_interrupt, (res[0]->flags & IRQF_TRIGGER_MASK) | IRQF_SHARED, "k2", ((void*)res[0]))){
        printk(KERN_ERR "button.c: Can't allocate irq %ud\n", res[0]->start);
        return -EBUSY;
    }
	if (request_irq(res[1]->start, button_interrupt, (res[1]->flags & IRQF_TRIGGER_MASK) | IRQF_SHARED, "k3", ((void*)res[1]))){
        printk(KERN_ERR "button.c: Can't allocate irq %ud\n", res[1]->start);
        return -EBUSY;
    }
	if (request_irq(res[2]->start, button_interrupt, (res[2]->flags & IRQF_TRIGGER_MASK) | IRQF_SHARED, "k4", ((void*)res[2]))){
        printk(KERN_ERR "button.c: Can't allocate irq %ud\n", res[2]->start);
        return -EBUSY;
    }
    
	DEBUG("irqnum:res[0]:%d, res[1]:%d, res[2]:%d\n", res[0]->start, res[1]->start, res[2]->start);

	return 0;
}
int key2_interrupt_remove(struct platform_device * pdevp){
	struct resource* res[3] = {};
	
	HARXON_DEBUG();
	res[0] = platform_get_resource(pdevp, IORESOURCE_IRQ, 0);
	res[1] = platform_get_resource(pdevp, IORESOURCE_IRQ, 1);
	res[2] = platform_get_resource(pdevp, IORESOURCE_IRQ, 2);

	free_irq(res[0]->start, (void*)(res[0]));
	free_irq(res[1]->start, (void*)(res[1]));
	free_irq(res[2]->start, (void*)(res[2]));
	
	return 0;
}

static const struct of_device_id of_id[] = {
	{
		.compatible = "fs4412-key",
	},
	{},
};

struct platform_driver pfd = {
	.probe = key2_interrupt_probe,
	.remove = key2_interrupt_remove,
	.driver = {
	//	.owner = THIS_MODULE,
		.name = "keyint_pfd",
		.of_match_table = of_match_ptr(of_id),

	},
};

static int __init button_init(void)
{
	HARXON_DEBUG();
	platform_driver_register(&pfd);

	return 0;
}

static void __exit button_exit(void)
{
	HARXON_DEBUG();
	platform_driver_unregister(&pfd);
}

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");

