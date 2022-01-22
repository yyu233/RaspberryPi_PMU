#include <linux/module.h>
#include <linux/kernel.h>

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
