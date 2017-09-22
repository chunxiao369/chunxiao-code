#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/string.h>
#include<linux/vmalloc.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

#define MAX_LEN 16
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Fortune Cookie Kernel Module");

static struct proc_dir_entry *proc_entry;
char msg[MAX_LEN] = {0};
int temp;

ssize_t fortune_write(struct file *filp, const char __user * buff, size_t count, loff_t *offp)
{
    char *data;
    printk("count is: %lu\n", count);
    printk("buf is: %s\n", buff);
    data = PDE_DATA(file_inode(filp));
    if (!data) {
        printk(KERN_INFO "Null data");
        return 0;
    }
    if (count > MAX_LEN) {
        count = MAX_LEN;
    }
    memset(msg, 0, MAX_LEN);
    copy_from_user(data, buff, count);
    return count;
}

ssize_t fortune_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
    char *data;
    data = PDE_DATA(file_inode(filp));
    if (!data) {
        printk(KERN_INFO "Null data");
        return 0;
    }
    if (count>temp) {
        count=temp;
    }
    temp=temp-count;
    printk("count is: %lu\n", count);

    copy_to_user(buf, data, count);
    if (count == 0)
        temp = strlen(msg);
    return count;
}

static const struct file_operations fortune_fops = {
    .owner = THIS_MODULE,
    .read = fortune_read,
    .write = fortune_write,
};

int init_fortune_module(void)
{
    int ret = 0;

    strcpy(msg, "Hello World\n");
    temp = strlen(msg);
    //proc_entry = create_proc_entry("fortune", 0, NULL);
    proc_entry = proc_create_data("fortune", 0, NULL, &fortune_fops, msg);
    if (proc_entry == NULL) {
        ret = -ENOMEM;
        printk(KERN_INFO "fortune :couldn't create proc entry\n");
    } else {
        /*
        proc_entry->read_proc = fortune_read;
        proc_entry->write_proc = fortune_write;
        */
        printk(KERN_INFO "fortune: Module load.\n");
    }
    return ret;
}

void cleanup_fortune_module(void)
{
    if (proc_entry) {
        remove_proc_entry("fortune", NULL);
    }
    printk(KERN_INFO "fortune:Module unload\n");
}

module_init(init_fortune_module);
module_exit(cleanup_fortune_module);
