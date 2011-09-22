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
void hold_do_work(void);    

static int gvalue;
static struct class *hold_class;
static int number;


void hold_do_work(void)
{
    int state;
    static int add = 0;
    struct kpp_dev *dev = kpp_devp;
    //state = gpio_get_value(HOLD_IRQ);
    state = (add++) % 2; 
    if (state == 1) {
        gvalue = 10;
        number++;
        if (number == 1) {
            if (dev->async_queue) 
                kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
        }
        schedule_work(&hold_wq);
    } else {
        gvalue = -10;
        number = 0;
        //enable_irq(gpio_to_irq(HOLD_IRQ));
        if (dev->async_queue)
            kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
    }
    msleep(100);
}


static irqreturn_t hold_isr(int irq, void *dev_id)
{
    int state;
    static int add = 0;
    mdelay(5);
    //state = gpio_get_value(HOLD_IRQ);
    state = (add++) % 2; 
    if (state == 1) {
        disable_irq(irq);
        schedule_work(&hold_wq);
    }
    return IRQ_HANDLED;
}


static int kpp_fasync(int fd, struct file *filp, int mode)   
{                                                            
    struct kpp_dev *dev = filp->private_data;                
    return fasync_helper(fd, filp, mode, &dev->async_queue);
}                                                            


static int kpp_release(struct inode *inode, struct file *filp)
{                            
    kpp_fasync(-1, filp, 0);
    return 0;                
}                            


static int kpp_open(struct inode *inode, struct file *filp)
{
    filp->private_data = kpp_devp;
    return 0;
}

static int kpp_ioctl(struct inode *inode, struct file *filp, unsigned
        int cmd, unsigned long arg)
{
    int value = gvalue;
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
    .ioctl   = kpp_ioctl,
    .fasync = kpp_fasync,
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

    hold_class = class_create(THIS_MODULE, "hold_class");
    if (IS_ERR(hold_class)) {
        printk("<1> create class error\n");
        return -1;
    }
    class_device_create(hold_class, NULL, devno, NULL, "hold");
}


static int __init kpp_init(void)
{
    int hold_irq;
    register_kpp();

    /*register hold-key irq interrupt */
    //hold_irq = gpio_to_irq(HOLD_IRQ);
    set_irq_type(hold_irq, IRQ_TYPE_EDGE_RISING);
    if (request_irq(hold_irq, hold_isr, IRQF_SAMPLE_RANDOM, "hold-key", NULL)) {
        printk(KERN_ERR "Request HOLD IRQ error !\n");
        return -EINVAL;
    }

    /*init work queue for hold irq */
    INIT_WORK(&hold_wq, (void (*)(void *)) hold_do_work);

    printk("Solomon Systech Limited\n");
    return 0;
}


static void __exit kpp_cleanup(void)
{
    int hold_irq;// = gpio_to_irq(HOLD_IRQ);
    free_irq(hold_irq, NULL);

    cdev_del(&kpp_devp->cdev);
    unregister_chrdev_region(devno, 1);
    class_device_destroy(hold_class, devno);
    class_destroy(hold_class);
}


module_init(kpp_init);
module_exit(kpp_cleanup);
MODULE_DESCRIPTION("Magus Keypad Driver");
MODULE_LICENSE("GPL");



