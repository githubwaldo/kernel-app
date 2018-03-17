#include <linux/init.h>
#include <linux/module.h>
#include <linux/bio.h>//struct bio
#include <linux/blkdev.h>
#include <linux/fs.h>

/* 创建一个虚拟磁盘 */
#define RAMDISK_SIZE  (16 * 1024 *1024)
#define SECTOR_BITS   9
char ramdisk[RAMDISK_SIZE];

static struct gendisk *disk;

static void ramdisk_make_request(struct request_queue *queue, struct bio *bio)
{
    struct bio_vec *bvec;
    unsigned short iter;
    void *user_mem;
    unsigned long phys_mem = (unsigned long) &ramdisk[bio->bi_sector << SECTOR_BITS];
    unsigned long phys_end = (unsigned long) &ramdisk[RAMDISK_SIZE];
    
    /* 遍历 */
    bio_for_each_segment(bvec, bio, iter) {
        if (unlikely(phys_mem + bvec->bv_len >= phys_end)) {
            printk("==bio_io_error===%lu=\n", phys_mem);
            bio_io_error(bio);
            return;
        }
        user_mem = page_address(bvec->bv_page) + bvec->bv_offset;
        
        if (bio_data_dir(bio) == READ)
            memcpy(user_mem, (void *)phys_mem, bvec->bv_len);
        else //WRITE
            memcpy((void *)phys_mem, user_mem, bvec->bv_len);
            
        phys_mem += bvec->bv_len;
    }
    bio_endio(bio, 0);
}

struct block_device_operations ramdisk_fops = {
    .open = NULL,
    .release = NULL,
    .rw_page = NULL,
    .ioctl = NULL,
    .compat_ioctl = NULL,
    .check_events = NULL,
    .media_changed = NULL,
    .unlock_native_capacity = NULL,
    .revalidate_disk = NULL,
    .getgeo = NULL,
    .swap_slot_free_notify = NULL,
    .owner = THIS_MODULE,
    .pr_ops = NULL,
};

static int __init ramdisk_driver_init(void)
{
    /* 创建、初始化并添加gendisk结构 */
    disk = alloc_disk(1);
    if (!disk)
        return -ENOMEM;
    
    /* 分配请求队列 */
    disk->queue = blk_alloc_queue(GFP_KERNEL);
    if (!disk->queue) {
        put_disk(disk);
        return -ENOMEM;
    }
    blk_queue_make_request(disk->queue, ramdisk_make_request);
    
    strcpy(disk->disk_name, "ramdisk");
    disk->major = 300;
    disk->first_minor = 0;
    disk->fops = &ramdisk_fops;
    /* 这里设置每个扇区大小 */
    set_capacity(disk, 1 << SECTOR_BITS);
    add_disk(disk);
    
    return 0;
}

static void __exit ramdisk_driver_exit(void)
{
    blk_cleanup_queue(disk->queue);
    del_gendisk(disk);
    put_disk(disk);
}

module_init(ramdisk_driver_init);
module_exit(ramdisk_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichun");
MODULE_DESCRIPTION("A simple block device driver");
MODULE_VERSION("V1.0");
MODULE_ALIAS("ramdisk_driver");