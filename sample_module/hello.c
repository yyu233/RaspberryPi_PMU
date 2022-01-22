/*
 *  hello.c - The simplest kernel module.
 */
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */

/* Get rid of error message of missing module license */
MODULE_LICENSE("GPL");

int init_module(void)
{
    printk(KERN_INFO "CSE237A: Hello world.\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "CSE237A: Goodbye world.\n");
}
