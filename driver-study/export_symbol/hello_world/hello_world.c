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
    u64 time;

    /*
    char *a = kmalloc(33, GFP_KERNEL);
    time = get_jiffies_64();
    printk("%p.\n", &time);
    printk("%p.\n", a);
    printk("%p.\n", hello_init);
    kfree(a);

    printk("1. jiffies: %lu, HZ: %d, get_jiffies_64: %llu.\n", jiffies, HZ, time);
    printk(KERN_ALERT "Hello, world\n");
    printk("2. jiffies: %lu.\n", jiffies);
    */
    printk("hello init.\n");
    return 0;
}

void embedway_test()
{
    printk("embedway_test.\n");
}

static void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
    printk("jiffies: %lu.\n", jiffies);
}

EXPORT_SYMBOL(embedway_test);
module_init(hello_init);
module_exit(hello_exit);

