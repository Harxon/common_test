#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_irq.h>

#include <linux/interrupt.h>
#include <linux/input.h>


#define Debug() printk(KERN_INFO "(%s):%d\n", __func__, __LINE__)
#define Debug_err() printk(KERN_ERR"ERR!!!:(%s):%d\n", __func__, __LINE__)

/* linux/of.h	或者是irq.c
 *
 *extern struct device_node *of_find_compatible_node(
 *								struct device_node *from,//从该设备节点的下一个节点开始搜索,==NULL
 *								const char *type, //需要匹配的节点类型，==NULL
 *								const char *compat);
 */

 static struct device_node* dev_nodp;
 static int irq_num[4];
 static struct input_dev* input_devp = NULL;
 static int cnt = 0;

static irqreturn_t k234_irq_handler(int irq_num, void* dev){
	cnt++;
	if(cnt>REL_MAX)cnt=0;

	switch(irq_num){
		case 167:{
			input_report_rel(input_devp, REL_X, cnt);
			printk(KERN_INFO "irq_num:%d, dev:%d\n", irq_num, *(int*)dev);
			break;
		}
		case 168:{
			printk(KERN_INFO "irq_num:%d, dev:%d\n", irq_num, *(int*)dev);
			break;
		}
		case 169:{
			input_report_abs(input_devp, ABS_X, cnt);
			printk(KERN_INFO "irq_num:%d, dev:%d\n", irq_num, *(int*)dev);
			break;
		}
		default:{
			break;
		}
	}
	input_sync(input_devp);
	
	return IRQ_HANDLED;
}

static int input_k234_interrupt_init(void){
	int i = 0;
	int err = 0;
	char irq_name[32] = {};
	Debug();

	dev_nodp = of_find_compatible_node(NULL, NULL, "fs4412-k234");
	//dev_nodp = of_find_compatible_node(NULL, NULL, "fs4412-k2");
	if(dev_nodp){
		printk("name:%s, full_name:%s\n", dev_nodp->name, dev_nodp->full_name);
	}else{
		Debug_err();
		goto err_find_node;
	}
	Debug();

	while(0 != (irq_num[i] = irq_of_parse_and_map(dev_nodp, i))){
		Debug();
		
		if(irq_num[i]){
			printk("irq_num:%d\n", irq_num[i]);
		}else{}
		
		memset(irq_name, 0, sizeof(irq_name));
		snprintf(irq_name, sizeof(irq_name), "k%d", irq_num[i]);
		
		printk("type:%#x\n", irq_get_trigger_type(irq_num[i]));
		err = request_irq(irq_num[i], k234_irq_handler, (irq_get_trigger_type(irq_num[i]) & IRQ_TYPE_SENSE_MASK) | IRQF_SHARED, irq_name, (void*)(&(irq_num[i])));
		i++;
		if(err){
			Debug_err();
			return -1;
		}else{}
	}		
	Debug();
	
	if(NULL == (input_devp = input_allocate_device())){
		Debug_err();
		goto err_to_free_irq;
	}

	input_devp->name = "input_harxon";
	input_devp->phys = "input_phys_harxon";
	
	set_bit(EV_REL, input_devp->evbit);
	set_bit(EV_ABS, input_devp->evbit);
	set_bit(REL_X, input_devp->relbit);
	set_bit(ABS_X, input_devp->absbit);
	input_set_abs_params(input_devp, ABS_X, 0, 255, 4, 8);
	
	if(input_register_device(input_devp)){
		Debug_err();
		goto err_to_input_register_device;
	}

	
	return 0;
err_to_input_register_device:
	input_free_device(input_devp);
err_to_free_irq:
	i = 0;
	while(irq_num[i]){
		free_irq(irq_num[i], (void*)&(irq_num[i]));
		i++;
	}
err_find_node:
	return 0;
}
static void input_k234_interrupt_exit(void){
	int i = 0;

	
	input_unregister_device(input_devp);
	input_free_device(input_devp);
	
	while(irq_num[i]){
		free_irq(irq_num[i], (void*)&(irq_num[i]));
		i++;
	}
	Debug();
}

module_init(input_k234_interrupt_init);
module_exit(input_k234_interrupt_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Handsome");
MODULE_DESCRIPTION("K2 K3 K4 available for input subsystem");
