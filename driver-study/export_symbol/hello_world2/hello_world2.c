/*
 * how to use jiffies
 * low 32 bit of value by get_jiffies_64 is jiffies
 *
 */
#include <linux/init.h>                             
#include <linux/module.h>
MODULE_LICENSE("GPL");

static int hello2_init(void)
{
    extern embedway_test();
    /*
    u64 time;
    char *a = kmalloc(33, GFP_KERNEL);
    time = get_jiffies_64();
    printk("%p.\n", &time);
    printk("%p.\n", a);
    kfree(a);

    printk("1. jiffies: %lu, HZ: %d, get_jiffies_64: %llu.\n", jiffies, HZ, time);
    printk(KERN_ALERT "Hello, world\n");
    printk("2. jiffies: %lu.\n", jiffies);
    */
    embedway_test();
    printk("hello2 init.\n");
    return 0;
}

static void hello2_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
    printk("jiffies: %lu.\n", jiffies);
}

module_init(hello2_init);
module_exit(hello2_exit);

