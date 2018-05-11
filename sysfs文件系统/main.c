#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>

#define power_attr(_name)                       \
static struct kobj_attribute _name##_attr = {   \
    .attr	= {                                 \
        .name = __stringify(_name),             \
        .mode = 0644,                           \
    },                                          \
    .show	= _name##_show,                     \
    .store	= _name##_store,                    \
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("mysysfs");
MODULE_VERSION("V1.0");
MODULE_ALIAS("a simple module");
static int trace_enabled;
/* --------------------------state file begin---------------------------------*/
static ssize_t state_show(struct kobject *kobj,
                    struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", trace_enabled);
}
static ssize_t state_store(struct kobject *kobj,
                    struct kobj_attribute *attr,
                    const char *buf, size_t n)
{
    int val;

    if (sscanf(buf, "%d", &val) == 1) {
        trace_enabled = !!val;
        return n;
    }
    return -EINVAL;
}
power_attr(state);
/* ----------------------------state file end---------------------------------*/

/* --------------------------jiffies file begin-------------------------------*/
static ssize_t jiffies_show(struct kobject *kobj,
                    struct kobj_attribute *attr, char *buf)
{
    if(trace_enabled)
        return sprintf(buf, "%ld\n", jiffies);
    return 0;
}
static ssize_t jiffies_store(struct kobject *kobj,
                    struct kobj_attribute *attr,
                    const char *buf, size_t n)
{
    return -EINVAL;
}
power_attr(jiffies);
/* --------------------------jiffies file end---------------------------------*/

static struct attribute * g[] = {
    &state_attr.attr,
    &jiffies_attr.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = g,
};

struct kobject *mystats_kobj;
static int __init mysysfs_init(void)
{
    int error;
    
    printk(KERN_ERR "#### hello!\n");
    /* 创建sys目录 */
    mystats_kobj = kobject_create_and_add("mystats", NULL);
    if (!mystats_kobj)
        return -ENOMEM;
    /* 在mystats下创建多个节点 */
    error = sysfs_create_group(mystats_kobj, &attr_group);
    if(error)
        return error;
    
    return 0;
}

static void __exit mysysfs_exit(void)
{
    sysfs_remove_group(mystats_kobj, &attr_group);
    kobject_put(mystats_kobj);
    printk(KERN_ERR "#### goodbye!\n");
}

module_init(mysysfs_init);
module_exit(mysysfs_exit);
