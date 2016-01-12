#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/string.h>
#include<linux/vmalloc.h>
#include<asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Fortune Cookie Kernel Module");

static struct proc_dir_entry *proc_entry;
DEFINE_MUTEX(board_info_mutex);

int fortune_write(struct file *filp, const char __user * buff, unsigned long len, void *data)
{
    mutex_lock(&board_info_mutex);
    mutex_unlock(&board_info_mutex);
    return 0;
}

int fortune_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    static int counter = 0;
    mutex_lock(&board_info_mutex);
    len = sprintf(page, "%s, %d\n", "abcdefg", counter);
    if (eof) {
        *eof = 1;
    }
    counter++;
    mutex_unlock(&board_info_mutex);
    return len;
}

int init_fortune_module(void)
{
    int ret = 0;
    proc_entry = create_proc_entry("fortune", 0, NULL);
    if (proc_entry == NULL) {
        ret = -ENOMEM;
        printk(KERN_INFO "fortune :couldn't create proc entry\n");
    } else {
        proc_entry->read_proc = fortune_read;
        proc_entry->write_proc = fortune_write;
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
