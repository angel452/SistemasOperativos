/*
* hello−2.c − Demonstrating the module_init() and module_exit() macros.
* This is preferred over using init_module() and cleanup_module().
*/

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/init.h> /* Needed for the macros */

static int __init inicio(void)
{
    printk(KERN_INFO "Hello Angel 2\n");
    return 0;
}

static void __exit fin(void)
{
    printk(KERN_INFO "Goodbye Angel 2\n");
}

MODULE_LICENSE("GPL");

module_init(inicio);
module_exit(fin);