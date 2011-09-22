#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x89e24b9c, "struct_module" },
	{ 0x10a6fca9, "kill_fasync" },
	{ 0x4086729e, "register_chrdev" },
	{ 0x37e74642, "get_jiffies_64" },
	{ 0xf3b39202, "mod_timer" },
	{ 0x7d11c268, "jiffies" },
	{ 0xd0b91f9b, "init_timer" },
	{ 0xa5808bbf, "tasklet_init" },
	{ 0x6651ba1a, "fasync_helper" },
	{ 0x1b7d4074, "printk" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0x4827a016, "del_timer" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3B7AE119E472B8854BFDE46");
