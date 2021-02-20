#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#define UART_CONTROL_MAJOR 200

static int g_uart_control_major = UART_CONTROL_MAJOR;
module_param(g_uart_control_major, int, S_IRUGO);

struct uart_control_dev {
    struct cdev cdev;
};

struct uart_control_dev *p_uart_control_dev;

static int my_open(struct inode *inode, struct file *pFile)
{
    return 0;
}

static int my_release(struct inode *inode, struct file *pFile) 
{
    return 0;
}

static long my_ioctl(struct file *pFile, unsigned int cmd, unsigned long arg)
{
    return 0;
}

const struct file_operations uart_control_ops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};

static void uart_control_set_up(struct uart_control_dev *dev, int index)
{
    int err ,devno = MKDEV(g_uart_control_major, 0);

    cdev_init(&dev->cdev, &uart_control_ops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1);

    if (err) {
        printk(KERN_NOTICE "uart control set up err(%d)", err);
    }
}

static int __init uart_control_init(void)
{
    int ret;
    dev_t devno = MKDEV(g_uart_control_major, 0);

    if (g_uart_control_major) {
        ret = register_chrdev_region(devno, 1, "uart control");
    } else {
        ret = alloc_chrdev_region(&devno, 0, 1, "uart control");
        g_uart_control_major = MAJOR(devno);
    }

    if (ret < 0) {
        return ret;
    }

    p_uart_control_dev = kzalloc(sizeof(struct uart_control_dev), GFP_KERNEL);
    if (!p_uart_control_dev) {
        ret = -ENOMEM;
        goto fail_malloc;
    }
    uart_control_set_up(p_uart_control_dev, 0);
    
    return 0;
    
fail_malloc:
    unregister_chrdev_region(devno, 1);
    return ret;
}

static void __exit uart_control_exit(void)
{
    cdev_del(&p_uart_control_dev->cdev);
    kfree(p_uart_control_dev);
    unregister_chrdev_region(MKDEV(g_uart_control_major, 0), 1);
}

module_init(uart_control_init);
module_exit(uart_control_exit);
MODULE_AUTHOR("LIU ZHEN BO");
MODULE_LICENSE("GPL");
