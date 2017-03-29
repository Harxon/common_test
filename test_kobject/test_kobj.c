#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/slab.h>

static struct kobject* kobjp = NULL;


/*
ssize_t	test_kobj_ops_show(struct kobject* kobj, struct attribute * attr , char* buf){

	printk(KERN_EMERG "test_kobj_ops_show done!\n");
	return 0;
}
ssize_t	test_kobj_ops_store(struct kobject* kobj, struct attribute* attr, const char* buf, size_t size){

	printk(KERN_EMERG "test_kobj_ops_store done!\n");
	return 0;
}


static struct sysfs_ops test_kobj_ops = {
	.show = test_kobj_ops_show,
	.store = test_kobj_ops_store,
};
*/
void test_kobj_release(struct kobject* kobj){
	
	kobject_put(kobj);
	kobject_del(kobj);
	kfree(kobj);
	printk(KERN_EMERG "test_kobj_release done!\n");
}

static struct attribute attr = {
		.name = "test_attr0",
		.mode = S_IWUSR,
};

static struct kobj_type ktype = {
	.release = test_kobj_release,
};

static struct kset test_kset = {
	struct list_head list;
	spinlock_t list_lock;
	struct kobject kobj;
	const struct kset_uevent_ops *uevent_ops;
}

int test_kobj_init(void){
	int err = 0;

	
	err = kset_register(&test_kset);
	kobjp = kzalloc(sizeof(struct kobject), GFP_ATOMIC);
	err = kobject_init_and_add(kobjp, &ktype, NULL, "test_kobj");
	printk(KERN_INFO "kobject_init_and_add err: %d\n",err);
	sysfs_create_file(kobjp, &attr);


	printk(KERN_EMERG "test_kobj_init success!\n");
	printk(KERN_EMERG "kobjp->name:%s\n",kobjp->name);
	return 0;
}

void test_kobj_exit(void){

	sysfs_remove_file(kobjp, &attr);
	kobject_put(kobjp);
	kobject_del(kobjp);
	kfree(kobjp);
	printk(KERN_EMERG "test_kobj_exit done!\n");
}

module_init(test_kobj_init);
module_exit(test_kobj_exit);

MODULE_LICENSE("GPL");
