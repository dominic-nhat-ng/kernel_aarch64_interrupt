#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // for copy_to_user
#include <asm/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nhat Nguyen");
MODULE_DESCRIPTION("Simple DAIFSet Register Writer");

#define MY_MAJOR 95
#define DEVICE_NAME "disable_interrupt"

static int driver_open(struct inode *device_file, struct file *instance) {
    printk(KERN_INFO "DAIFSet writer - open was called\n");
    return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
    printk(KERN_INFO "DAIFSet writer - close was called\n");
    return 0;
}

static ssize_t daifset_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    printk(KERN_INFO "DAIFSet writer - masking SError/IRQ exceptions\n"); 

    // Set DAIF register bits to mask SError and IRQ exceptions
    asm volatile("msr DAIFSet, #2");

    return count; // Return the number of bytes processed
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .write = daifset_write, 
};

static int __init write_daifset_init(void) {
    int ret;

    ret = register_chrdev(MY_MAJOR, DEVICE_NAME, &fops);
    if (ret < 0) {
        printk(KERN_ERR "DAIFSet writer - failed to register device number (Major: %d)\n", MY_MAJOR);
        return ret;
    }

    printk(KERN_INFO "DAIFSet writer - registered device number Major: %d, Minor: %d\n", MY_MAJOR, 0);
    return 0;
}

static void __exit write_daifset_exit(void) {
    unregister_chrdev(MY_MAJOR, DEVICE_NAME);
    printk(KERN_INFO "DAIFSet writer module unloaded\n");
}

module_init(write_daifset_init);
module_exit(write_daifset_exit);

