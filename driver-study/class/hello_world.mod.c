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
	{ 0xb5513e49, "class_device_create" },
	{ 0x2e1de6c1, "class_create" },
	{ 0x5e22fdec, "cdev_add" },
	{ 0xdf2724d7, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x1b7d4074, "printk" },
	{ 0x70ecc9b2, "cdev_del" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x4870cf59, "class_destroy" },
	{ 0xf26c4b72, "class_device_destroy" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5670B5103607BDDE6EEB953");
