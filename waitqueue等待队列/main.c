#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>//wait_event
#include <linux/uaccess.h>//copy_from_user

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("wait_queue");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");

static char name[256];
char *ptr = NULL;

/* 定义并初始化一个等待队列 */
static DECLARE_WAIT_QUEUE_HEAD(read_wq);
static int name_read(char *page, char **start, off_t offset, int count, int *eof, void *data)
{
    /* 定义并初始化一个等待队列 */
    //wait_queue_head_t read_wq;
    //init_waitqueue_head(&read_wq);

    /* 等待事件 */
    //wait_event(read_wq, (ptr != NULL));
    //wait_event_interruptible(read_wq, (ptr != NULL));
    wait_event_killable(read_wq, (ptr != NULL));

    /* 等待 */
    //sleep_on(&read_wq);//wake_up(&read_wq)
    //interruptible_sleep_on(&read_wq);//wake_up_interruptible(&read_wq)

    *eof = 1;
    return sprintf(page, "%s", ptr);
}

static int name_write(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
    if (count < 1) {
        return -EINVAL;
    }
    /* 写字符串 */
    if (copy_from_user(name, buffer, count))
        return -EFAULT;

    /* 写控制位 */
    //char mode;
    //int number;
    //if (get_user(mode, buffer))
    //    return -EFAULT;
    //if (mode >= '0' && mode <= '9')
    //    number = mode - '0';
    //else
    //    return count;
    //printk(KERN_ERR "Number:%d\n", number);

    ptr = name;

    /* 唤醒等待队列 */
    wake_up(&read_wq);
    //wake_up_interruptible(&read_wq);
    
    return count;
}

static struct proc_dir_entry *name_proc;
static int __init wait_queue_init(void)
{
    printk(KERN_ERR "#### hello!\n");
    name_proc = create_proc_entry("name", 0666, NULL);
    if(!name_proc) {
       printk(KERN_ERR "#### Can't create:%s\n","name");
       return 0;
    }
    name_proc->write_proc = name_write;
    name_proc->read_proc = name_read;
    name_proc->data = NULL;

    return 0;
}

static void __exit wait_queue_exit(void)
{
    remove_proc_entry("name",NULL);
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(wait_queue_init);
module_exit(wait_queue_exit);
