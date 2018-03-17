#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("my_waitqueue");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");

struct student
{
    int id;
    char *name;
    int score;
    struct list_head list;
};

struct student zhao = {
    .id = 1,
    .name = "zhao",
    .score = 60,
};

struct student qian = {
    .id = 2,
    .name = "qian",
    .score = 70,
};

struct student sun = {
    .id = 3,
    .name = "sun",
    .score = 80,
};

struct student li = {
    .id = 4,
    .name = "li",
    .score = 90,
};
static int __init work_init(void)
{
    printk(KERN_ERR "#### hello!\n");
    /* 定义链表头，并初始化 */
    LIST_HEAD(student_list);
    
    /* 插入元素 */
    list_add_tail(&zhao.list, &student_list);
    list_add_tail(&qian.list, &student_list);
    list_add_tail(&sun.list, &student_list);
    list_add_tail(&li.list, &student_list);

    /* 遍历链表一 */
    struct list_head *pos;
    struct student *ptr;
    list_for_each(pos,&student_list)
    {
        ptr = list_entry(pos,struct student,list);
        printk(KERN_ERR "ID:%d\tName:%s\tScore:%d\n",ptr->id, ptr->name, ptr->score);
    }

    /* 删除元素 */
    list_del(&li.list);

    /* 遍历链表二 */
    struct student *pos1;
    list_for_each_entry(pos1, &student_list, list)
    {
        printk(KERN_ERR "ID:%d\tName:%s\tScore:%d\n",pos1->id, pos1->name, pos1->score);
    }

    if(list_empty(&student_list)) {
        printk(KERN_ERR "#### empty!\n");
    }

    return 0;
}

static void __exit work_exit(void)
{
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(work_init);
module_exit(work_exit);
