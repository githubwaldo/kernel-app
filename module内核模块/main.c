#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("my module");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");

static int age = 18;
static char *name = "lichun";
module_param(age, int, S_IRUGO);
module_param(name, charp, S_IRUGO);

static int __init my_module_init(void)
{
   printk(KERN_INFO "#### hello! %s\n", name);
   return 0;
}

static void __exit my_module_exit(void)
{
   printk(KERN_INFO "#### goodbye! %d\n", age);
}

int func(int a, int b)
{
    printk(KERN_INFO "In Func: %s...\n",__func__);
    return a+b;
}
//EXPORT_SYSBOL(func);
EXPORT_SYSBOL_GPL(func);

module_init(my_module_init);
module_exit(my_module_exit);
