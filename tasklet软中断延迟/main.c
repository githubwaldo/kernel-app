#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("mytasklet_queue");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");


static void mytasklet_handler(unsigned long data)
{
    printk(KERN_ERR "#### mytasklet_handler\n");
}

static struct tasklet_struct my_tasklet;
static int __init mytasklet_init(void)
{
    printk(KERN_ERR "#### hello!\n");
    /* 初始化Tastlet */
    tasklet_init(&my_tasklet, mytasklet_handler, (unsigned long)0);
    /* 调度Tastlet */
    tasklet_schedule(&my_tasklet);

    return 0;
}

static void __exit mytasklet_exit(void)
{
    tasklet_kill(&my_tasklet);
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(mytasklet_init);
module_exit(mytasklet_exit);
