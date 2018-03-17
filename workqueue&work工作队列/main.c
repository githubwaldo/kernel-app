#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("my_workqueue");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");

struct workqueue_struct *workqueue;

static void work_handler(struct work_struct *work)
{
    msleep(5000);
    printk(KERN_ERR "#### work_handler!\n");
}


/* ����һ��work */
//static DECLARE_WORK(mywork, work_handler);
static int __init work_init(void)
{
    printk(KERN_ERR "#### hello!\n");
    /* ����һ���������� */
    workqueue = create_workqueue("my_workqueue");
    //workqueue = create_singlethread_workqueue("my_workqueue");
    if(!workqueue)
        return -ENOMEM;
    
    /* ����һ��work */
    static struct work_struct mywork;
    INIT_WORK(&mywork,work_handler);
    
    /* ��� */
    queue_work(workqueue,&mywork);
    //schedule_work(&mywork);// ��events����
    return 0;
}

static void __exit work_exit(void)
{
    flush_workqueue(workqueue);//�ȴ�workqueue����workȫ��ִ��
    destroy_workqueue(workqueue);//�ͷ�workqueue
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(work_init);
module_exit(work_exit);
