#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>

#define DEVNAME "hello" 
#define DEFAULT_MSG "xuchunxiao" 
MODULE_LICENSE("GPL");
static dev_t dev;
char hello_buf[64];
static struct class *hello_class; 
static struct cdev *hello_cdev;
static struct file_operations hello_fops = {
    read: NULL,
    write: NULL,
    open: NULL, 
    release: NULL, 
    ioctl: NULL,
    fasync: NULL,
};


static int __init hello_init(void)
{
    int error;

    error = alloc_chrdev_region(&dev, 0, 2, "hello");
    if (error)
    {
        printk("hello: alloc_chardev_region failed!\n");
        goto out;
    }
    hello_cdev = cdev_alloc();
    if (hello_cdev == NULL)
    {
        printk("hello: alloc cdev failed!\n");
        error = -ENOMEM;
        goto out_chrdev;
    }
    hello_cdev->ops = &hello_fops;
    hello_cdev->owner = THIS_MODULE;
    error = cdev_add(hello_cdev, dev, 1);
    if (error)
    {
        printk("hello: cdev_add failed!\n");
        goto out_cdev;
    }
    hello_class = class_create(THIS_MODULE, DEVNAME); 
    if (IS_ERR(hello_class)) 
    { 
        error = PTR_ERR(hello_class); 
        goto out_chrdev; 
    } 
    class_device_create(hello_class, NULL, dev, NULL, DEVNAME); 
    memset (hello_buf, 0, sizeof(hello_buf));
    memcpy(hello_buf, DEFAULT_MSG, sizeof(DEFAULT_MSG));
    printk("hello: Hello World!\n");
    return 0;

out_cdev:
    cdev_del(hello_cdev);
out_chrdev:
    unregister_chrdev_region(hello_cdev->dev, 2);
out:
    return error;
}

static void __exit hello_exit(void)
{
    class_device_destroy(hello_class, dev); 
    class_destroy(hello_class); 
    unregister_chrdev_region(hello_cdev->dev, 2);
    cdev_del(hello_cdev);
    printk("hello: Goodbye World\n");
}

module_init(hello_init);
module_exit(hello_exit);

