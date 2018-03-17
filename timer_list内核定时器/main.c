#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("my timer");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");

struct timer_list timer;
static void timer_handler(unsigned long data)
{
    printk(KERN_ERR "#### %ld!\n",data);
    mod_timer(&timer, jiffies+3*HZ);//修改并添加
}

static int __init timer_init(void)
{
   printk(KERN_ERR "#### hello!\n");

   init_timer(&timer);
   timer.data = 5;//unsigned long
   timer.function = timer_handler;
   timer.expires = jiffies + 10*HZ;
   add_timer(&timer);//添加定时器

   return 0;
}

static void __exit timer_exit(void)
{
    if(timer_pending(&timer))
        del_timer(&timer);//移除未超时的定时器
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(timer_init);
module_exit(timer_exit);
