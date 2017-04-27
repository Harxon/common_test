#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#define Debug() printk(KERN_INFO "(%s):%d\n", __func__, __LINE__)

#define Harxon 1

#ifdef Harxon
#define test_func()			 \
	do{						\
		Debug();				\
		printk("dowhile\n");\
static int test_module_init(void){	\
	test_func();\
	Debug();\
	return 0;\
}								\
void test_module_exit(void){   \
	Debug();					\
}						\
module_init(test_module_init);\
module_exit(test_module_exit);\
MODULE_LICENSE("GPL");\
	}while(0)
#endif
test_func();
