#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>//copy_from_user

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("proc");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");

char name[10]="lichun";

static struct proc_dir_entry *platform_proc;

static int mystat_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    return sprintf(buf, "name = %s\n"
                        "age = %d\n",
                        name, 18);
}


static int proc_read_node(char *page, char **start, off_t off, int count, int *eof, void *data) 
{
//count:用户希望读取字节
//offset:读数据的便宜量
    return sprintf(page, "name = %s\n"
                        "age = %d\n",
                        name, 18);
}

static int proc_write_node(struct file *file, const char *buffer, unsigned long count, void *data)
{
//count:用户写入的字节数
//buffer:指向用户数据(位于用户空间)
    int len;
    if(count > sizeof(name))
        len = sizeof(name);
    else
        len = count;

    if(copy_from_user(name, buffer, len))
        return -EFAULT;

    name[len-1] = '\0';
    return len;
}

static int __init proc_init(void)
{
   printk(KERN_ERR "#### hello!\n");

   /* 创建proc文件夹 */
   platform_proc = proc_mkdir("platform", NULL);
   if(!platform_proc) {
       printk(KERN_ERR "Can't create: /proc/%s!\n","platform");
       return 0;
   }

   /* 创建只读proc节点 */
   struct proc_dir_entry *mystat_proc;
   mystat_proc = create_proc_read_entry("mystat", 0, platform_proc, mystat_read, NULL);
   if(!mystat_proc) {
       printk(KERN_ERR "Can't create: /proc/%s/%s!\n", platform_proc->name, "mystat");
       return 0;
   }
   
   /* 创建读写proc节点 */
   struct proc_dir_entry *mynode_proc;
    mynode_proc = create_proc_entry("mynode", 0644, platform_proc);
    if(!mynode_proc) {
       printk(KERN_ERR "Can't create: /proc/%s/%s!\n", platform_proc->name, "mynode");
       return 0;
    }
    mynode_proc->data = name;
    mynode_proc->read_proc = proc_read_node;
    mynode_proc->write_proc = proc_write_node;

    return 0;
}

static void __exit proc_exit(void)
{
    /* 删除节点1 */
    if(platform_proc)
        remove_proc_entry("mystat", platform_proc);
    /* 删除节点2 */
    if(platform_proc)
        remove_proc_entry("mynode", platform_proc);
    /* 删除目录 */
    remove_proc_entry("platform", NULL);
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(proc_init);
module_exit(proc_exit);
