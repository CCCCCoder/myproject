#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>

struct hello_dev {
    struct cdev cdev;
};

static const struct file_operations hello_fops = {
};

struct hello_dev *gphello_dev;
int ghello_major = 230;

static int __init hello_init(void)
{
    int err = 0;
    ///< 设备号    
    dev_t devno = MKDEV(ghello_major, 0);
    if (ghello_major)
        err = register_chrdev_region(devno, 1, "hello");
    else {
        err = alloc_chrdev_region(&devno, 0, 1, "hello");
        ghello_major = MAJOR(devno);
    }
    
    if (err < 0) {
        goto end;
    }
    
    gphello_dev = kzalloc(sizeof(struct hello_dev), GFP_KERNEL);
    if (gphello_dev == NULL) {
        err = -ENOMEM;
        goto free;
    }
    ///< 初始化设备结构
    cdev_init(&gphello_dev->cdev, &hello_fops);
    gphello_dev->cdev.owner = THIS_MODULE;
    ///< 向系统增加设备
    err = cdev_add(&gphello_dev->cdev, devno, 1);
    printk(KERN_ALERT "Hello world!\n");
end:
    return err;
free:
    unregister_chrdev_region(devno, 1);
    goto end;
}

static void __exit hello_exit(void)
{
    kfree(gphello_dev);
    unregister_chrdev_region(MKDEV(ghello_major, 0), 1);
    printk(KERN_ALERT "Goodbye,cruel world!");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("MDAXIA");

