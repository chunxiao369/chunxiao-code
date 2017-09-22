/*
 * how to use jiffies
 * low 32 bit of value by get_jiffies_64 is jiffies
 *
 */

#include <linux/kernel.h>
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

#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm-generic/siginfo.h>
#include <uapi/asm-generic/signal.h>
#include <uapi/asm-generic/siginfo.h>
//#include <asm/arch-magus/hardware.h>
//#include <asm/arch-magus/irqs.h>
//#include <asm/gpio.h>
#include <asm/param.h>

#define KPPMAJOR        240
#define MODULE_NAME     "kpp"
#define HOLD_IRQ        MAGUS_GPIO_HOLD
#define KPP_GET         _IOR('f', 2, int)

//<<<<<Global Variable
struct kpp_dev {                       
    struct cdev cdev;                  
    struct fasync_struct *async_queue;
};                                     
struct kpp_dev *kpp_devp;              

//work queue struct
struct work_struct hold_wq;

static int kpp_release(struct inode *inode, struct file *filp)
{                            
    return 0;                
}                            

static int kpp_open(struct inode *inode, struct file *filp)
{
    filp->private_data = kpp_devp;
    return 0;
}

static long kpp_ioctl(struct file *filp, unsigned
        int cmd, unsigned long arg)
{
    int value = 88881234;
    printk("ioctl coming.\n");
    switch (cmd) {
        case KPP_GET:
            put_user(value, (int __user *) arg);
            break;
        default:
            return -ENOTTY;
    }
    return 0;
}

static struct file_operations g_kpp_fops = {
    .owner   = THIS_MODULE,
    .unlocked_ioctl   = kpp_ioctl,
    .open    = kpp_open,
    .release = kpp_release,
};


static void kpp_setup_cdev(struct kpp_dev *dev, int index)
{
    int devno = MKDEV(KPPMAJOR, index);
    cdev_init(&dev->cdev, &g_kpp_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &g_kpp_fops;
    cdev_add(&dev->cdev, devno, 1);
}

static dev_t devno;


static void register_kpp(void)
{
    devno = MKDEV(KPPMAJOR, 0);
    register_chrdev_region(devno, 1, MODULE_NAME);
    kpp_devp = kmalloc(sizeof (struct kpp_dev), GFP_KERNEL);
    memset(kpp_devp, 0, sizeof (struct kpp_dev));
    kpp_setup_cdev(kpp_devp, 0);
}


static int __init kpp_init(void)
{
    register_kpp();
    printk("embedway cxxu init\n");
    return 0;
}


static void __exit kpp_cleanup(void)
{
    cdev_del(&kpp_devp->cdev);
    unregister_chrdev_region(devno, 1);
    printk("embedway cxxu cleanup\n");
}


module_init(kpp_init);
module_exit(kpp_cleanup);
MODULE_DESCRIPTION("Magus Keypad Driver");
MODULE_LICENSE("GPL");



