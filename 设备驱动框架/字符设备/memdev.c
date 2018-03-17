#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>//struct file inode
#include <linux/cdev.h>//struct cdev
#include <asm/uaccess.h>//copy_to_user、copy_from_user

int major = 0;
struct cdev cdev;

/* 创建两个虚拟设备 */
struct memdev {
    char data[4096];
};
struct memdev memdevs[2];

int memdev_open(struct inode *inode, struct file *filp) {
    /* 获取次设备号，从而确定操作memdev1还是memdev2 */
    int minor = MINOR(inode->i_rdev);
    if (minor >= 2)
        return -ENODEV;
    
    /*将设备描述结构指针赋值给文件私有数据指针*/
    filp->private_data = &memdevs[minor];
    
    /* 成功返回0 失败返回失败原因如，-ENODEV */
    return 0;
}

int memdev_release(struct inode *inode, struct file *filp) {
    return 0;
}

static ssize_t memdev_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos) {
    size_t count = size;
    struct memdev *dev = filp->private_data;
    
    /* 判断访问位置是否有效 */
    if (*ppos >= 4096)
        return 0;
    
    /* 不可超memdev访问 */
    if (*ppos + size > 4096)
        count = 4096 - *ppos;
    
    /*读数据到用户空间*/
    if (copy_to_user(buf, (void*)(dev->data + *ppos), count)) {
        return -EFAULT;
    }
    *ppos += count;//将文件当前位置向后移
    
    /* 成功返回实际读取字节数 */
    return count;
}

static ssize_t memdev_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos) {
    unsigned int count = size;
    struct memdev *dev = filp->private_data;

    /* 判断访问位置是否有效 */
    if (*ppos >= 4096)
        return 0;
    
    /* 不可超memdev访问 */
    if (*ppos + size > 4096)
        count = 4096 - *ppos;

    /*从用户空间写入数据*/  
    if (copy_from_user(dev->data + *ppos, buf, count)) {
        return -EFAULT;
    }
    *ppos += count;//将文件当前位置向后移
    
    /* 成功返回实际写入字节数 */
    return count;
}

static loff_t memdev_llseek(struct file *filp, loff_t offset, int whence) {
    loff_t newpos;
    
    switch(whence) {
        case 0: /* SEEK_SET */
            newpos = offset;//从文件头开始定位
            break;
        case 1: /* SEEK_CUR */
            newpos = filp->f_pos + offset;//从文件中间定位
            break;
        case 2: /* SEEK_END */
            newpos = 4096 -1 + offset;//从文件尾开始定位，由于是从0开始，所以要减1
            break;
        default: /* can't happen */
            return -EINVAL;
    }
    /* 判断newpos的合法性 */
    if ((newpos < 0) || (newpos > 4096))
        return -EINVAL;
    
    filp->f_pos = newpos;
    /* 成功返回偏移后的f_pos */
    return newpos;
}

/*
static int memdev_mmap(struct file*filp, struct vm_area_struct *vma)  
{  //获得设备结构体指针
      struct mem_dev *dev = filp->private_data; 
        
      vma->vm_flags |= VM_IO;  
      vma->vm_flags |= (VM_DONTEXPAND | VM_DONTDUMP);  
  
       
      if (remap_pfn_range(vma,vma->vm_start,virt_to_phys(dev->data)>>PAGE_SHIFT, vma->vm_end - vma->vm_start, vma->vm_page_prot))  
          return  -EAGAIN;  
                  
      return 0;  
}  
  */
  
/* memdev设备的操作集 */
static struct file_operations memdev_fops =
{
    .owner = THIS_MODULE,
    .open  = memdev_open,          // 对应系统调用open,null表示永远成功,1.初始化 2.标明次设备号
    .release = memdev_release,     // 对应系统调用close
    .read  = memdev_read,          // 对应系统调用read,copy_to_user()
    .write = memdev_write,         // 对应系统调用write,copy_from_user()
    .llseek = memdev_llseek,       // 对应系统调用seek
    .poll = NULL,                  // 对应系统调用select
    .mmap = NULL,                  // 对应系统调用mmap
    //.ioctl = NULL,                 // 对应系统调用ioctl 2.6.32前
    .compat_ioctl = NULL,  // 对应系统调用ioctl 2.6.32及之后
};

static int memdev_driver_init(void)
{
    int result;
    dev_t dev;
    
    /* 注册或分配主设备号相同，次设备号连续的多个设备 */
    /* 通过/proc/devices可以查看被占用的主设备号 */
    dev = MKDEV(major, 0);
    if (MAJOR(dev))
        result = register_chrdev_region(dev, 2, "memdev");
    else {
        result = alloc_chrdev_region(&dev, 0, 2, "memdev");
        major = MAJOR(dev);
    }
    if(result) {
        printk(KERN_ERR "memdev_driver: failed to register chrdev region\n");
        return 0;
    }

    /*初始化并添加cdev结构*/
    cdev_init(&cdev, &memdev_fops);
    cdev.owner = THIS_MODULE;
    //cdev.ops = &memdev_fops; //在cdev_init已经完成赋值
    cdev_add(&cdev, MKDEV(major, 0), 2);//将/proc/devices的设备和cdev关联

    return 0;
}

static void memdev_driver_exit(void)
{
    cdev_del(&cdev);/*注销设备*/
    unregister_chrdev_region(MKDEV(major, 0), 2);/*释放设备号*/
}

module_init(memdev_driver_init);
module_exit(memdev_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("A simple character device driver");
MODULE_VERSION("V1.0");
MODULE_ALIAS("memdev_driver");