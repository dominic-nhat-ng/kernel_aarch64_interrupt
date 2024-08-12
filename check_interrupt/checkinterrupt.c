#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // for copy_to_user
#include <asm/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nhat Nguyen");
MODULE_DESCRIPTION("Check DAIF Register - Interrupt Status");

#define MY_MAJOR 93
#define DEVICE_NAME "check_interrupt"

static int driver_open(struct inode *device_file, struct file *instance) {
    printk(KERN_INFO "DAIF checker - open was called\n");
    return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
    printk(KERN_INFO "DAIF checker - close was called\n");
    return 0;
}

static ssize_t daif_check_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    unsigned long daif;
    char result[128];
    int len;

    // Read the DAIF register
    asm volatile("mrs %0, DAIF" : "=r"(daif));

    // Check if IRQ and FIQ are masked
    len = snprintf(result, sizeof(result), 
                   "DAIF register: 0x%lx\nIRQ masked: %s\nFIQ masked: %s\n", 
                   daif, 
                   (daif & (1 << 7)) ? "Yes" : "No",  // IRQ is masked if I bit (bit 7) is set
                   (daif & (1 << 6)) ? "Yes" : "No"); // FIQ is masked if F bit (bit 6) is set

    // Copy the result to user space
    if (copy_to_user(buf, result, len)) {
        return -EFAULT;
    }

    return len; // Return the number of bytes read
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = daif_check_read, 
};

static int __init daif_checker_init(void) {
    int ret;

    ret = register_chrdev(MY_MAJOR, DEVICE_NAME, &fops);
    if (ret < 0) {
        printk(KERN_ERR "DAIF checker - failed to register device number (Major: %d)\n", MY_MAJOR);
        return ret;
    }

    printk(KERN_INFO "DAIF checker - registered device number Major: %d, Minor: %d\n", MY_MAJOR, 0);
    return 0;
}

static void __exit daif_checker_exit(void) {
    unregister_chrdev(MY_MAJOR, DEVICE_NAME);
    printk(KERN_INFO "DAIF checker module unloaded\n");
}

module_init(daif_checker_init);
module_exit(daif_checker_exit);

