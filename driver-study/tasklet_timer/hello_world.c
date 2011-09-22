/*
 * how to use jiffies
 * low 32 bit of value by get_jiffies_64 is jiffies
 *
 */
#include <linux/init.h>                             
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>
#include <linux/string.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <net/dst.h>

MODULE_LICENSE("Dual BSD/GPL");

struct tasklet_struct tasklet_test;
struct timer_list timer_test;

static void test_func(unsigned long a)
{
    a = a;
    printk("tasklet test func is running.\n");
}

static int test_tasklet_init(void)
{
    tasklet_init(&tasklet_test, test_func, 0);
    return 0;
}

static void test_do_timer(unsigned long arg)
{
    printk("test do timer coming, jiffies: %lu.\n", jiffies);
    mod_timer(&timer_test, jiffies + HZ);
    return;
}
static int test_timer_init(void)
{
    init_timer(&timer_test);
    timer_test.data = 0;
    timer_test.function = test_do_timer;
    mod_timer(&timer_test, jiffies + HZ);

    return 0;
}

static int hello_init(void)
{
    u64 time;
   
    test_tasklet_init();
    test_timer_init();
    time = get_jiffies_64();
    printk("1. jiffies: %lu, HZ: %d, get_jiffies_64: %llu.\n", jiffies, HZ, time);
    printk(KERN_ALERT "Hello, world\n");
    printk("2. jiffies: %lu.\n", jiffies);

    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
    printk("jiffies: %lu.\n", jiffies);
    del_timer(&timer_test);
}

module_init(hello_init);
module_exit(hello_exit);

