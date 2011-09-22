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
	{ 0x89b301d4, "param_get_int" },
	{ 0x98bd6f46, "param_set_int" },
	{ 0x97448b2e, "check_disk_change" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xb1ce3a82, "bio_endio" },
	{ 0x674dd92f, "end_that_request_last" },
	{ 0xf77499e6, "elv_dequeue_request" },
	{ 0x5155cd2d, "end_that_request_first" },
	{ 0x16b86be, "kunmap_atomic" },
	{ 0x8f9ae706, "kmap_atomic" },
	{ 0xc9c86fc0, "elv_next_request" },
	{ 0x1887e9fc, "end_request" },
	{ 0x1bcd461f, "_spin_lock" },
	{ 0x91eea053, "add_disk" },
	{ 0x25da070, "snprintf" },
	{ 0x8e34540, "alloc_disk" },
	{ 0x3e224d55, "blk_queue_hardsect_size" },
	{ 0x5287bedb, "blk_init_queue" },
	{ 0x9f55d388, "blk_queue_make_request" },
	{ 0xf7b4f388, "blk_alloc_queue" },
	{ 0xd0b91f9b, "init_timer" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0x1b7d4074, "printk" },
	{ 0x37a0cba, "kfree" },
	{ 0xeac1c4af, "unregister_blkdev" },
	{ 0x2fd1d81c, "vfree" },
	{ 0x495dc8ce, "blk_cleanup_queue" },
	{ 0xaeb40fa6, "blk_put_queue" },
	{ 0x717f87fe, "put_disk" },
	{ 0xb1575f16, "del_gendisk" },
	{ 0xc659d5a, "del_timer_sync" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1FD951016DA0F55EDD47933");
