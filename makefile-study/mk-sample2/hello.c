/*
 * how to use jiffies
 * low 32 bit of value by get_jiffies_64 is jiffies
 *
 */
#include <linux/init.h>                             
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
    extern int ta(void);
    extern int tb(void);
    u64 time;
    time = get_jiffies_64();
    printk("%p.\n", &time);
    printk("%p.\n", hello_init);

    printk("1. jiffies: %lu, HZ: %d, get_jiffies_64: %llu.\n", jiffies, HZ, time);
    printk(KERN_ALERT "Hello, world\n");
    printk("2. jiffies: %lu.\n", jiffies);
    ta();
    tb();
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
    printk("jiffies: %lu.\n", jiffies);
}

module_init(hello_init);
module_exit(hello_exit);

