#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <asm/io.h>
#include "efm32gg.h"

#define DRIVER_NAME "TDT4258_Gamepad_Driver"
#define GPIO_IRQ_EVEN 17
#define GPIO_IRQ_ODD 18

//========== Local Function Declarations ==========//
static int __init setup(void);
static void __exit teardown(void);
static irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs);
static ssize_t driver_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
static int driver_open (struct inode *inode, struct file *filp);
static int driver_release (struct inode *inode, struct file *filep);
static int driver_fasync_helper(int fd, struct file *filp, int mode);

//========== Local Variable Declarations ==========//
struct cdev driver_cdev;
struct class* class;
struct fasync_struct* queue;
dev_t device_number;

// Maps the file operations to local functions
static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .read = driver_read,
    .write = driver_write,
    .open = driver_open,
    .release = driver_release,
    .fasync = driver_fasync_helper
};

static int __init setup(void)
{
	printk("Setting up TDT4258 Gamepad Driver...\n");
	
	// Allocating device numbers
	if(alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME) < 0){
		printk(KERN_ALERT "Error allocating driver numbers...\n");
		return -1;
	}
	
	// IO Port Allocation
	if(request_mem_region(GPIO_PC_MODEL, 4, DRIVER_NAME) == NULL){
		printk(KERN_ALERT "Could not allocate necessary memory regions. Setup failed!\n");
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_DIN, 1, DRIVER_NAME) == NULL){
		printk(KERN_ALERT "Could not allocate necessary memory regions. Setup failed!\n");
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_DOUT, 1, DRIVER_NAME) == NULL){
		printk(KERN_ALERT "Could not allocate necessary memory regions. Setup failed!\n");
		return -1;
	}
	
	// Virtual Memory Mapping
	ioremap(GPIO_PC_MODEL, 4);
	ioremap(GPIO_PC_DIN, 1);
	ioremap(GPIO_PC_DOUT, 1);
	
	// GPIO Button Setup
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xFF, GPIO_PC_DOUT);
	
	// Add driver to OS interrupt list
	request_irq(GPIO_IRQ_EVEN, (irq_handler_t) gpio_interrupt_handler, 0, DRIVER_NAME, &driver_cdev);
	request_irq(GPIO_IRQ_ODD, (irq_handler_t) gpio_interrupt_handler, 0, DRIVER_NAME, &driver_cdev);
	
	// Initializing cdev and add to kernel driver list
	cdev_init(&driver_cdev, &driver_fops);
	driver_cdev.owner = THIS_MODULE;
	cdev_add(&driver_cdev, device_number, 1);
	
	// Enable interrupts
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	iowrite32(0xFF, GPIO_EXTIFALL);
	iowrite32(0xFF, GPIO_IEN);
	
	class = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(class, NULL, device_number, NULL, DRIVER_NAME);
	
	printk("Setting up TDT4258 Gamepad Driver Complete!\n");
	return 0;
}

static void __exit teardown(void)
{
	printk("Tearing down TDT4258 Gamepad Driver...\n");
}

irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
	iowrite32(ioread32(GPIO_IF), GPIO_IFC);
	if (queue) {
		kill_fasync(&queue, SIGIO, POLL_IN);
	}
    
    return IRQ_HANDLED;
}

static ssize_t driver_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	int value = ioread32(GPIO_PC_DIN);
	copy_to_user(buff, &value, 4);
	return 1;
}

static ssize_t driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	return 1;
}
static int driver_open (struct inode *inode, struct file *filp)
{
	return 0;
}

static int driver_release (struct inode *inode, struct file *filp)
{
	return 0;
}

static int driver_fasync_helper(int fd, struct file *filp, int mode)
{
	return fasync_helper(fd, filp, mode, &queue);
}

module_init(setup);
module_exit(teardown);

MODULE_DESCRIPTION("TDT4258 Gamepad Driver");
MODULE_LICENSE("GPL");

